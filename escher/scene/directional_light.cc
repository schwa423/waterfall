// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/scene/directional_light.h"

#include <glm/gtc/matrix_transform.hpp>
#include <utility>

namespace escher {

DirectionalLight::DirectionalLight() {
}

DirectionalLight::DirectionalLight(glm::vec3 source,
                                   glm::vec3 target,
                                   float radius)
  : source_(std::move(source)),
    target_(std::move(target)),
    radius_(radius) {
}

DirectionalLight::~DirectionalLight() {
}

glm::mat4 DirectionalLight::GetProjectionMatrix(
    const ViewingVolume& viewing_volume) const {
  float width = viewing_volume.size().width();
  float height = viewing_volume.size().height();

  // Assume the light source is coplanar with the XY ground plane and emits
  // perfectly collimated rays.  To find the shaded portion of the scene,
  // we project it orthographically into the same viewing volume used for
  // rendering but apply a skew to the position of each object in proportion
  // with its elevation.  Rendering this projection can be used to fill
  // buffers with information about the occluder of each pixel.
  float delta_x = source_.x - target_.x;
  float delta_y = source_.y - target_.y;
  float delta_z = source_.z - target_.z;
  ESCHER_DCHECK(delta_z > 0.001f);
  return viewing_volume.GetProjectionMatrix() *
         glm::mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, delta_x / -delta_z,
                   delta_y / -delta_z, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
}

}  // namespace escher
