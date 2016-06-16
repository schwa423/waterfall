// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/scene/viewing_volume.h"

#include <utility>

namespace escher {

ViewingVolume::ViewingVolume() {
}

ViewingVolume::ViewingVolume(SizeI size, float near, float far)
  : size_(std::move(size)), near_(near), far_(far) {
}

ViewingVolume::~ViewingVolume() {
}

Matrix4 ViewingVolume::GetProjectionMatrix() const {
  return Matrix4::ortho(0.0f,
                        size_.width(),
                        size_.height(),
                        0.0f,
                        -near_,
                        -far_);
}

}  // namespace escher
