// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/renderer.h"

#include "escher/gl/extensions.h"
#include "util/shader_utils.h"

namespace escher {
namespace {

constexpr char g_vertex_shader[] = SHADER_SOURCE(
  attribute vec4 a_position;
  uniform mat4 u_matrix;

  void main() {
    gl_Position = u_matrix * a_position;
  }
);

constexpr char g_fragment_shader[] = SHADER_SOURCE(
  precision mediump float;
  uniform vec4 u_color;

  void main() {
    gl_FragColor = u_color;
  }
);

}  // namespace

SolidColorShader::SolidColorShader() {
}

SolidColorShader::~SolidColorShader() {
  if (program_) {
    glDeleteProgram(program_);
    program_ = 0;
  }
}

bool SolidColorShader::Compile() {
  program_ = CompileProgram(g_vertex_shader, g_fragment_shader);
  if (!program_)
    return false;

  matrix_ = 0;
  color_ = 1;
  glBindUniformLocation(program_, matrix_, "u_matrix");
  glBindUniformLocation(program_, color_, "u_color");

  position_ = 0;
  return true;
}

}  // namespace escher
