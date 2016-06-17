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
  return viewing_volume.GetProjectionMatrix() * glm::lookAt(
      source_, target_, glm::vec3(0.0f, 1.0f, 0.0));
}

}  // namespace escher
