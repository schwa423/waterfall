// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>
#include <vector>

#include "escher/base/macros.h"

namespace escher {

class SizeI {
 public:
  SizeI();
  SizeI(int width, int height);

  int width() const { return width_; }
  int height() const { return height_; }

  float aspect_ratio() const {
    return float(width_) / height_;
  }

  int area() const { return width_ * height_; }

  glm::vec2 AsVec2() const;
  bool Equals(const SizeI& size) const;

 private:
  int width_ = 0;
  int height_ = 0;
};

}  // namespace escher
