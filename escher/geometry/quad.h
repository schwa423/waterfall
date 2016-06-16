// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "util/Vector.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>

namespace escher {

class Quad {
 public:
  Quad();
  Quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3);

  static Quad CreateFromRect(Vector2 position, Vector2 size, float z);

  const float* data() const {
    return reinterpret_cast<const float*>(this);
  }

  static const GLushort* GetIndices();
  static GLsizei GetIndexCount();

 private:
  Vector3 p[4] = {};
};

}  // namespace escher
