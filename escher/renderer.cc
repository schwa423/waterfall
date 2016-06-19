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

namespace escher {
namespace {

// Material design places objects from 0.0f to 24.0f. We inflate that range
// slightly to avoid clipping at the edges of the viewing volume.
constexpr float kNear = 25.0f;
constexpr float kFar = -1.0f;

// Key light parameters.
constexpr float kKeyLightRadius = 600.0f;
constexpr float kKeyLightElevation = 600.0f;

}  // namespace

Renderer::Renderer() {}

Renderer::~Renderer() {}

bool Renderer::Init() {
  InitGLExtensions();

  if (!solid_color_shader_.Compile())
    return false;

  if (!shadow_shader_.Compile())
    return false;

  if (!depth_shader_.Compile())
    return false;

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  return true;
}

void Renderer::SetSize(SizeI size) {
  if (!shadow_map_.SetSize(size)) {
    std::cerr << "Failed to allocate shadow map of size (" << size.width()
              << ", " << size.height() << ")." << std::endl;
    exit(1);
  }

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

  glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_.frame_buffer().id());
  glViewport(0, 0, shadow_map_.size().width(), shadow_map_.size().height());
  glClear(GL_DEPTH_BUFFER_BIT);
  DrawModelWithDepthShader(model, matrix);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, stage_.size().width(), stage_.size().height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  DrawModelWithShadowShader(model, matrix);
}

void Renderer::DrawModelWithDepthShader(const Model& model,
                                        const glm::mat4& light_matrix) {
  glUseProgram(depth_shader_.program().id());
  glEnableVertexAttribArray(depth_shader_.position());
  glUniformMatrix4fv(depth_shader_.matrix(), 1, GL_FALSE, &light_matrix[0][0]);

  for (const auto& obj : model.objects()) {
    glVertexAttribPointer(depth_shader_.position(), 3, GL_FLOAT, GL_FALSE, 0,
                          obj->positions());
    glDrawElements(GL_TRIANGLES, obj->index_count(), GL_UNSIGNED_SHORT,
                   obj->indices());
  }
}

void Renderer::DrawModelWithShadowShader(const Model& model,
                                         const glm::mat4& matrix) {
  glUseProgram(shadow_shader_.program().id());
  glEnableVertexAttribArray(shadow_shader_.position());
  glUniformMatrix4fv(shadow_shader_.matrix(), 1, GL_FALSE, &matrix[0][0]);
  auto viewing_volume = stage_.viewing_volume();
  glUniform3f(shadow_shader_.viewing_volume(), viewing_volume.size().width(),
              viewing_volume.size().height(), viewing_volume.depth());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadow_map_.texture().id());
  glUniform1i(shadow_shader_.depth_map(), 0);

  for (const auto& obj : model.objects()) {
    const glm::vec4& color = obj->material()->color();
    glUniform4f(shadow_shader_.color(), color.x, color.y, color.z, color.w);
    glVertexAttribPointer(shadow_shader_.position(), 3, GL_FLOAT, GL_FALSE, 0,
                          obj->positions());
    glDrawElements(GL_TRIANGLES, obj->index_count(), GL_UNSIGNED_SHORT,
                   obj->indices());
  }
}

}  // namespace escher
