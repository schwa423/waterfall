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

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

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

  app_bar_ = Quad::CreateFromRect(glm::vec2(0.0f, 0.0f),
                                  glm::vec2(size.width(), 56.0f),
                                  4.0f);
  canvas_ = Quad::CreateFromRect(glm::vec2(0.0f, 0.0f), size.AsVec2(), 0.0f);
  fab_ = Quad::CreateFromRect(glm::vec2(size.width() - 56.0f - 16.0,
                                      size.height() - 56.0f - 16.0),
                              glm::vec2(56.0f, 56.0f),
                              6.0f);

  stage_.set_viewing_volume(ViewingVolume(std::move(size), kNear, kFar));
  stage_.set_key_light(DirectionalLight(
      glm::vec3(size.width() / 2.0f, 0.0f, kKeyLightElevation),
      glm::vec3(size.width() / 2.0f, kKeyLightElevation, 0.0f),
      kKeyLightRadius));
  stage_.set_fill_light(DomeLight(
      glm::vec3(size.width() / 2.0f, size.height() / 2.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
      2.0f * std::max(size.width(), size.height())));
}

void Renderer::DrawSolidColorQuad(const Quad& quad, const glm::vec4& color) {
  glUniform4f(shadow_shader_.color(), color.x, color.y, color.z, color.w);
  DrawQuad(shadow_shader_.position(), quad);
}

void Renderer::DrawQuad(GLint position, const Quad& quad) {
  glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, quad.data());
  glDrawElements(GL_TRIANGLES, Quad::GetIndexCount(),
                 GL_UNSIGNED_SHORT, Quad::GetIndices());
}

void Renderer::Render(TimePoint frame_time) {
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_.frame_buffer().id());
  glViewport(0, 0, shadow_map_.size().width(), shadow_map_.size().height());
  glClear(GL_DEPTH_BUFFER_BIT);
  glUseProgram(depth_shader_.program().id());
  glm::mat4 light_matrix = stage_.key_light().GetProjectionMatrix(
      stage_.viewing_volume());
  glUniformMatrix4fv(depth_shader_.matrix(), 1, GL_FALSE, &light_matrix[0][0]);
  glEnableVertexAttribArray(depth_shader_.position());
  DrawQuad(depth_shader_.position(), canvas_);
  DrawQuad(depth_shader_.position(), app_bar_);
  DrawQuad(depth_shader_.position(), fab_);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, stage_.size().width(), stage_.size().height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shadow_shader_.program().id());
  glm::mat4 matrix = stage_.viewing_volume().GetProjectionMatrix();
  glEnableVertexAttribArray(shadow_shader_.position());
  glUniformMatrix4fv(shadow_shader_.matrix(), 1, GL_FALSE, &matrix[0][0]);
  glUniformMatrix4fv(shadow_shader_.light_matrix(), 1, GL_FALSE, &light_matrix[0][0]);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadow_map_.texture().id());
  glUniform1i(shadow_shader_.shadow_map(), 0);
  DrawSolidColorQuad(canvas_, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  DrawSolidColorQuad(app_bar_, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  DrawSolidColorQuad(fab_, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

}  // namespace escher
