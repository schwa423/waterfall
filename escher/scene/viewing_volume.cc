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
  // TODO(abarth): Consider flipping top and bottom. We currently use math-style
  // with y increasing from bottom to top as opposed to UI-style with y
  // increasing from top to bottom.
  return Matrix4::ortho(0.0f, size_.width(), 0.0f, size_.height(), near_, far_);
}

}  // namespace escher
