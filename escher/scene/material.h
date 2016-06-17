// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>

namespace escher {

class Material {
 public:
  Material();
  ~Material();

  void set_color(const glm::vec4& color) { color_ = color; }
  const glm::vec4& color() const { return color_; }

 private:
  glm::vec4 color_;
};

}  // namespace escher
