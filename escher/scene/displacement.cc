// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/scene/displacement.h"

namespace escher {

Displacement::Displacement() {}

Displacement::~Displacement() {}

Displacement Displacement::MakeWave(const glm::vec2& start,
                                    const glm::vec2& end,
                                    float max) {
  Displacement displacement;
  displacement.type_ = Type::kWave;
  displacement.parameters_ = glm::vec4(start, end);
  displacement.max_ = max;
  return displacement;
}

}  // namespace escher
