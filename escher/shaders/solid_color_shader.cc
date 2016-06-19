// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/solid_color_shader.h"

#include "escher/shaders/uniforms.h"

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
  precision mediump float;
  uniform vec4 u_color;

  void main() {
    gl_FragColor = u_color;
  }
)GLSL";

}  // namespace

SolidColorShader::SolidColorShader() {
}

SolidColorShader::~SolidColorShader() {
}

bool SolidColorShader::Compile() {
  program_ = MakeUniqueProgram(g_vertex_shader, g_fragment_shader);
  if (!program_)
    return false;

  BindUniforms(program_.id(), {
    &matrix_,
    &color_,
  }, {
    "u_matrix",
    "u_color",
  });

  position_ = 0;
  return true;
}

}  // namespace escher
