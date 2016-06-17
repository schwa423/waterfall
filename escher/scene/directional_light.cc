// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/scene/directional_light.h"

#include <utility>

namespace escher {

DirectionalLight::DirectionalLight() {
}

DirectionalLight::DirectionalLight(glm::vec3 source,
                                   glm::vec3 target,
                                   float radius,
                                   float fov)
  : source_(std::move(source)),
    target_(std::move(target)),
    radius_(radius),
    fov_(fov) {
}

DirectionalLight::~DirectionalLight() {
}

glm::mat4 DirectionalLight::GetProjectionMatrix(
    const ViewingVolume& viewing_volume) const {
  // TODO(abarth): Actually compute the project matrix for the light.
  return glm::mat4();
}

}  // namespace escher
