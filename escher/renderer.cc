// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/renderer.h"

#include "escher/gl/extensions.h"
#include "util/Matrix.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <math.h>
#include <utility>

namespace escher {
namespace {

// Material design places objects from 0.0f to 24.0f. We inflate that range
// slightly to avoid clipping at the edges of the viewing volume.
constexpr float kNear = 25.0f;
constexpr float kFar = -1.0f;

// Key light parameters.
constexpr float kKeyLightFOV = M_PI/8.0;
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

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  return true;
}

void Renderer::SetSize(SizeI size) {
  quad_ = Quad::CreateFromRect(Vector2(10.0f, 5.0f),
                               Vector2(size.width() / 2.0f,
                                       size.height() / 2.0f),
                               0.0);
  stage_.set_viewing_volume(ViewingVolume(std::move(size), kNear, kFar));
  stage_.set_key_light(DirectionalLight(
      Vector3(size.width() / 2.0f, size.height(), kKeyLightElevation),
      Vector3(size.width() / 2.0f, size.height() - kKeyLightElevation, 0.0f),
      kKeyLightRadius,
      kKeyLightFOV));
  stage_.set_fill_light(DomeLight(
      Vector3(size.width() / 2.0f, size.height() / 2.0f, 0.0f),
      Vector3(0.0f, 0.0f, 1.0f),
      2.0f * std::max(size.width(), size.height())));
}

void Renderer::Render(TimePoint frame_time) {
  glViewport(0, 0, stage_.size().width(), stage_.size().height());
  glClear(GL_COLOR_BUFFER_BIT);

  Matrix4 matrix = stage_.viewing_volume().GetProjectionMatrix();

  glUniformMatrix4fv(solid_color_shader_.matrix(), 1, GL_FALSE, matrix.data);
  glUniform4f(solid_color_shader_.color(), 1.0f, 1.0f, 0.0f, 1.0f);

  glUseProgram(solid_color_shader_.program());

  glVertexAttribPointer(solid_color_shader_.position(), 3, GL_FLOAT, GL_FALSE,
                        0, quad_.data());
  glEnableVertexAttribArray(solid_color_shader_.position());

  glDrawElements(GL_TRIANGLES, Quad::GetIndexCount(),
                 GL_UNSIGNED_SHORT, Quad::GetIndices());
}

}  // namespace escher
