// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>

#include "escher/scene/binding.h"

namespace escher {

// Texture and shading properties to apply to a surface.
class Material {
 public:
  Material();
  ~Material();

  // Base color, default is none.
  void set_color(const Binding<glm::vec4>& color) { color_ = color; }
  const Binding<glm::vec4>& color() const { return color_; }

  // TODO(jeffbrown): Displacement, normals.

 private:
  Binding<glm::vec4> color_;
};

}  // namespace escher
