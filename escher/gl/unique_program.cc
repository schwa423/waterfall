// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/gl/unique_program.h"

#include "util/shader_utils.h"

namespace escher {

UniqueProgram MakeUniqueProgram(const std::string& vertex_shader,
                                const std::string& fragment_shader) {
  GLuint id = CompileProgram(vertex_shader, fragment_shader);
  UniqueProgram program;
  program.Reset(id);
  return program;
}

}  // namespace escher
