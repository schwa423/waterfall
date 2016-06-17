// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/uniforms.h"

#include <assert.h>
#include <iostream>

#include "escher/gl/extensions.h"

namespace escher {

void BindUniforms(GLuint program_id,
                  std::initializer_list<GLint*> locations,
                  std::initializer_list<const char*> names) {
  assert(locations.size() == names.size());
  auto location_iter = locations.begin();
  auto name_iter = names.begin();
  GLint next_location = 0;
  while (location_iter != locations.end() && name_iter != names.end()) {
    GLint* location = *location_iter++;
    *location = next_location++;
    glBindUniformLocation(program_id, *location, *name_iter++);
  }
}

}  // namespace escher
