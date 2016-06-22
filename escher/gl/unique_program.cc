// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/gl/unique_program.h"

#include <iostream>
#include <vector>

namespace escher {
namespace {

void LogProgramError(GLuint id, const char* prologue) {
#ifndef NDEBUG
  GLint length = 0;
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

  if (length > 1) {
    std::vector<GLchar> message(length);
    glGetProgramInfoLog(id, length, nullptr, message.data());
    // TODO(abarth): Switch to a reasonable logging system.
    std::cerr << prologue << message.data();
  } else {
    std::cerr << prologue << "<empty>" << std::endl;
  }
#endif
}

}  // namespace

UniqueShader MakeUniqueShader(GLenum type, const std::string& source) {
  UniqueShader shader;
  shader.Reset(glCreateShader(type));
  const char* string = source.c_str();
  glShaderSource(shader.id(), 1, &string, nullptr);
  glCompileShader(shader.id());

  GLint status = 0;
  glGetShaderiv(shader.id(), GL_COMPILE_STATUS, &status);

  if (!status) {
    LogProgramError(shader.id(), "shader compilation failed: ");
    shader.Reset();
  }

  return shader;
}

UniqueProgram MakeUniqueProgram(const std::string& vertex_shader,
                                const std::string& fragment_shader) {
  UniqueProgram program;
  program.Reset(glCreateProgram());

  UniqueShader vertex = MakeUniqueShader(GL_VERTEX_SHADER, vertex_shader);
  UniqueShader fragment = MakeUniqueShader(GL_FRAGMENT_SHADER, fragment_shader);
  if (!vertex || !fragment) {
    program.Reset();
    return program;
  }

  glAttachShader(program.id(), vertex.id());
  glAttachShader(program.id(), fragment.id());
  glLinkProgram(program.id());

  GLint status = 0;
  glGetProgramiv(program.id(), GL_LINK_STATUS, &status);

  if (!status) {
    LogProgramError(program.id(), "shader compilation failed: ");
    program.Reset();
  }

  return program;
}

}  // namespace escher
