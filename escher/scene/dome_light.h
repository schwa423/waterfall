// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>

#include "escher/scene/viewing_volume.h"

namespace escher {

class DomeLight {
 public:
  DomeLight();
  DomeLight(glm::vec3 center, glm::vec3 normal, float radius);
  ~DomeLight();

  // The center of the half-sphere emitting light.
  const glm::vec3& center() const { return center_; }

  // The direction from the center of the dome to the top of the half-sphere.
  const glm::vec3& normal() const { return normal_; }

  // The radius of the half sphere.
  float radius() const { return radius_; }

 private:
  glm::vec3 center_;
  glm::vec3 normal_;
  float radius_ = 0.0f;
};

}  // namespace escher
