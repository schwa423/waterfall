// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>

#include "escher/scene/viewing_volume.h"

namespace escher {

// Directional light is emitted from a particular point at infinity with some
// angular dispersion.
class DirectionalLight {
 public:
  DirectionalLight();
  DirectionalLight(glm::vec2 direction, float dispersion, float intensity);
  ~DirectionalLight();

  // The direction from which the light is received. The first coordinate is
  // theta (the the azimuthal angle) and the second coordinate is phi (the polar
  // angle).
  const glm::vec2& direction() const { return direction_; }

  // The amount of angular variance in the light, in radians.
  float dispersion() const { return dispersion_; }

  // The amount of light emitted.
  // TODO(abarth): In what units?
  float intensity() const { return intensity_; }

 private:
  glm::vec2 direction_;
  float dispersion_ = 0.0f;
  float intensity_ = 0.0f;
};

}  // namespace escher
