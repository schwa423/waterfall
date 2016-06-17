// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>
#include <vector>

namespace escher {

class Quad {
 public:
  Quad();
  Quad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

  static Quad CreateFromRect(glm::vec2 position, glm::vec2 size, float z);

  const float* data() const {
    return reinterpret_cast<const float*>(this);
  }

  static const GLushort* GetIndices();
  static GLsizei GetIndexCount();

 private:
  glm::vec3 p[4] = {};
};

}  // namespace escher
