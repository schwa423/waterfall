// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/renderer.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <iostream>
#include <math.h>
#include <utility>

#include "escher/gl/extensions.h"
#include "escher/textures/noise_texture.h"

namespace escher {
namespace {

constexpr bool kIlluminateScene = true;
constexpr bool kFilterLightingBuffer = true;

// Material design places objects from 0.0f to 24.0f. We inflate that range
// slightly to avoid clipping at the edges of the viewing volume.
constexpr float kNear = 25.0f;
constexpr float kFar = -1.0f;

// Key light parameters.
constexpr float kKeyLightRadius = 600.0f;
constexpr float kKeyLightElevation = 600.0f;

}  // namespace

Renderer::Renderer()
    : scene_buffer_(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
      lighting_buffer_(GL_COLOR_BUFFER_BIT) {}

Renderer::~Renderer() {}

bool Renderer::Init() {
  InitGLExtensions();

  if (!blit_shader_.Compile() || !depth_shader_.Compile() ||
      !illumination_shader_.Compile() || !lighting_filter_.Compile() ||
      !shadow_shader_.Compile() || !solid_color_shader_.Compile())
    return false;

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  noise_texture_ = MakeNoiseTexture(
      SizeI(ShadowShader::kNoiseSize, ShadowShader::kNoiseSize));

  return true;
}

void Renderer::SetSize(SizeI size) {
  if (stage_.size().Equals(size))
    return;

  if (!scene_buffer_.SetSize(size) || !lighting_buffer_.SetSize(size)) {
    std::cerr << "Failed to allocate frame buffer of size (" << size.width()
              << ", " << size.height() << ")." << std::endl;
    exit(1);
  }

  scratch_texture_ = MakeColorTexture(size);
  full_frame_ = Quad::CreateFromRect(glm::vec2(-1.0f, 1.0f),
                                     glm::vec2(2.0f, -2.0f), 0.0f);

  stage_.set_viewing_volume(ViewingVolume(std::move(size), kNear, kFar));
  stage_.set_key_light(
      DirectionalLight(glm::vec3(size.width() / 2.0f, 0.0f, kKeyLightElevation),
                       glm::vec3(size.width() / 2.0f, kKeyLightElevation, 0.0f),
                       kKeyLightRadius));
  stage_.set_fill_light(
      DomeLight(glm::vec3(size.width() / 2.0f, size.height() / 2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                2.0f * std::max(size.width(), size.height())));
}

void Renderer::Render(const Model& model) {
  glm::mat4 matrix = stage_.viewing_volume().GetProjectionMatrix();
  glViewport(0, 0, stage_.size().width(), stage_.size().height());

  glBindFramebuffer(GL_FRAMEBUFFER, scene_buffer_.frame_buffer().id());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  DrawModelWithSolidColorShader(model, matrix);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  ComputeIllumination();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  if (!kIlluminateScene) {
    Blit(lighting_buffer_.color().id());
  } else {
    glUseProgram(illumination_shader_.program().id());
    glUniform1i(illumination_shader_.scene(), 0);
    glUniform1i(illumination_shader_.lighting(), 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scene_buffer_.color().id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lighting_buffer_.color().id());
    glEnableVertexAttribArray(shadow_shader_.position());
    DrawFullFrameQuad(shadow_shader_.position());
  }
}

void Renderer::ComputeIllumination() {
  glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer_.frame_buffer().id());
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(shadow_shader_.program().id());
  glUniform1i(shadow_shader_.depth_map(), 0);
  glUniform1i(shadow_shader_.noise(), 1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, scene_buffer_.depth().id());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, noise_texture_.id());
  glEnableVertexAttribArray(shadow_shader_.position());
  DrawFullFrameQuad(shadow_shader_.position());

  if (kFilterLightingBuffer) {
    scratch_texture_ =
        lighting_buffer_.SetColorTexture(std::move(scratch_texture_));
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(lighting_filter_.program().id());
    glUniform1i(lighting_filter_.illumination_map(), 0);
    auto view_size = stage_.viewing_volume().size();
    glUniform2f(lighting_filter_.tap_stride(), 1.0f / view_size.width(), 0.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scratch_texture_.id());
    glEnableVertexAttribArray(lighting_filter_.position());
    DrawFullFrameQuad(shadow_shader_.position());

    scratch_texture_ =
        lighting_buffer_.SetColorTexture(std::move(scratch_texture_));
    glUniform2f(lighting_filter_.tap_stride(), 0.0f, 1.0f / view_size.height());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scratch_texture_.id());
    glEnableVertexAttribArray(lighting_filter_.position());
    DrawFullFrameQuad(shadow_shader_.position());
  }
}

void Renderer::Blit(GLuint texture_id) {
  glUseProgram(blit_shader_.program().id());
  glUniform1i(blit_shader_.source(), 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glEnableVertexAttribArray(blit_shader_.position());
  DrawFullFrameQuad(blit_shader_.position());
}

void Renderer::DrawModelWithSolidColorShader(const Model& model,
                                             const glm::mat4& matrix) {
  glUseProgram(solid_color_shader_.program().id());
  glEnableVertexAttribArray(solid_color_shader_.position());
  glUniformMatrix4fv(solid_color_shader_.matrix(), 1, GL_FALSE, &matrix[0][0]);

  for (const auto& obj : model.objects()) {
    const glm::vec4& color = obj->material()->color();
    glUniform4f(solid_color_shader_.color(), color.x, color.y, color.z,
                color.w);
    glVertexAttribPointer(solid_color_shader_.position(), 3, GL_FLOAT, GL_FALSE,
                          0, obj->positions());
    glDrawElements(GL_TRIANGLES, obj->index_count(), GL_UNSIGNED_SHORT,
                   obj->indices());
  }
}

void Renderer::DrawFullFrameQuad(GLint position) {
  glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, full_frame_.data());
  glDrawElements(GL_TRIANGLES, Quad::GetIndexCount(), GL_UNSIGNED_SHORT,
                 Quad::GetIndices());
}

}  // namespace escher
