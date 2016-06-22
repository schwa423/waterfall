// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <string>

#include "escher/gl/unique_object.h"

namespace escher {

typedef UniqueObject<glDeleteShader> UniqueShader;
UniqueShader MakeUniqueShader(GLenum type, const std::string& source);

typedef UniqueObject<glDeleteProgram> UniqueProgram;
UniqueProgram MakeUniqueProgram(const std::string& vertex_shader,
                                const std::string& fragment_shader);

}  // namespace escher
