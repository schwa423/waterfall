// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/scene/viewing_volume.h"
#include "util/Vector.h"

namespace escher {

class DomeLight {
 public:
  DomeLight();
  DomeLight(Vector3 center, Vector3 normal, float radius);
  ~DomeLight();

  // The center of the half-sphere emitting light.
  const Vector3& center() const { return center_; }

  // The direction from the center of the dome to the top of the half-sphere.
  const Vector3& normal() const { return normal_; }

  // The radius of the half sphere.
  float radius() const { return radius_; }

 private:
  Vector3 center_;
  Vector3 normal_;
  float radius_ = 0.0f;
};

}  // namespace escher
