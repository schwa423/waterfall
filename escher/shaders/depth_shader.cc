// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/depth_shader.h"

#include "escher/gl/extensions.h"

namespace escher {
namespace {

constexpr char g_vertex_shader[] = R"GLSL(
  attribute vec3 a_position;
  uniform mat4 u_matrix;

  void main() {
    gl_Position = u_matrix * vec4(a_position, 1.0);
  }
)GLSL";

constexpr char g_fragment_shader[] = R"GLSL(
  void main() { }
)GLSL";

}  // namespace

DepthShader::DepthShader() {
}

DepthShader::~DepthShader() {
}

bool DepthShader::Compile() {
  program_ = MakeUniqueProgram(g_vertex_shader, g_fragment_shader);
  if (!program_)
    return false;

  matrix_ = 0;
  glBindUniformLocation(program_.id(), matrix_, "u_matrix");

  position_ = 0;
  return true;
}

}  // namespace escher
