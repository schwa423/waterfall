// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/scene/dome_light.h"

#include <utility>

namespace escher {

DomeLight::DomeLight() {
}

DomeLight::DomeLight(Vector3 center, Vector3 normal, float radius)
  : center_(std::move(center)),
    normal_(std::move(normal)),
    radius_(radius) {
}

DomeLight::~DomeLight() {
}

}  // namespace escher
