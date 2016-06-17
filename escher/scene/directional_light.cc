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
  // TODO(abarth): Figure out how to compute these values analytically.
  float width = viewing_volume.size().width();
  float height = viewing_volume.size().height();
  float half_width = width * 0.5f;
  return glm::ortho<float>(-width * 0.6, width * 0.6, height, 0.0f, 200.0f,
                           1000.0f) *
         glm::lookAt<float>(glm::vec3(half_width, 600.0f, 700.0f),
                            glm::vec3(half_width, 0.0f, 100.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
}

}  // namespace escher
