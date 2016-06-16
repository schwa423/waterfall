// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/geometry/size_i.h"

namespace escher {

SizeI::SizeI() {}

SizeI::SizeI(int width, int height) : width_(width), height_(height) {
}

bool SizeI::Equals(const SizeI& size) {
  return width_ == size.width_ && height_ == size.height_;
}

}  // namespace escher
