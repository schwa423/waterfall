// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/renderer.h"

#include "escher/gl/extensions.h"
#include "util/Matrix.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <utility>

namespace escher {

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

bool Renderer::Init() {
  InitGLExtensions();

  if (!solid_color_shader_.Compile())
    return false;
  
  quad_ = Quad::CreateFromRect(Vector2(-0.5, -0.5), Vector2(1.0, 1.0), 0.0);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  return true;
}

void Renderer::SetSize(SizeI size) {
  size_ = std::move(size);
}

void Renderer::Render(TimePoint frame_time) {
  glViewport(0, 0, size_.width(), size_.height());
  glClear(GL_COLOR_BUFFER_BIT);

  Matrix4 matrix =
      Matrix4::perspective(60.0f, size_.aspect_ratio(), 1.0f, 20.0f) *
      Matrix4::translate(Vector3(0.0f, 0.0f, -2.0f));

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
