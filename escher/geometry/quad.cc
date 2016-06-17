// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/geometry/quad.h"

#include "escher/base/arraysize.h"

namespace escher {
namespace {

constexpr GLushort g_indices[] = {
  0, 2, 1,
  0, 3, 2,
};

}  // namespace

Quad::Quad() {}

Quad::Quad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
  p[0] = p0;
  p[1] = p1;
  p[2] = p2;
  p[3] = p3;
}

Quad Quad::CreateFromRect(glm::vec2 position, glm::vec2 size, float z) {
  return Quad(glm::vec3(position.x,          position.y,          z),
              glm::vec3(position.x + size.x, position.y,          z),
              glm::vec3(position.x + size.x, position.y + size.y, z),
              glm::vec3(position.x,          position.y + size.y, z));
}

const GLushort* Quad::GetIndices() {
  return g_indices;
}

GLsizei Quad::GetIndexCount() {
  return arraysize(g_indices);
}

}  // namespace escher