// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/scene/viewing_volume.h"

#include <glm/glm.hpp>

namespace escher {

class DirectionalLight {
 public:
  DirectionalLight();
  DirectionalLight(glm::vec3 source, glm::vec3 target, float radius, float fov);
  ~DirectionalLight();

  // The center of the area that emits the light.
  const glm::vec3& source() const { return source_; }

  // A location intersected by a light ray emitted from the center of the light.
  const glm::vec3& target() const { return target_; }

  // The radius of the area that emits the light.
  float radius() const { return radius_; }

  // The angular dispersion of the emitted light in radians.
  float fov() const { return fov_; }

  glm::mat4 GetProjectionMatrix(const ViewingVolume& viewing_volume) const;

 private:
  glm::vec3 source_;
  glm::vec3 target_;
  float radius_ = 0.0f;
  float fov_ = 0.0f;
};

}  // namespace escher
