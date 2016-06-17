// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <initializer_list>

namespace escher {

void BindUniforms(GLuint program_id,
                  std::initializer_list<GLint*> locations,
                  std::initializer_list<const char*> names);

}  // namespace escher
