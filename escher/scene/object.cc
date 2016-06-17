// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/scene/model.h"

namespace escher {

Object::Object(const Material* material) : material_(material) {
  ESCHER_DCHECK(material);
}

Object::~Object() {}

std::unique_ptr<Object> Object::CreateRect(const glm::vec2& position,
                                           const glm::vec2& size,
                                           float z,
                                           const Material* material) {
  std::unique_ptr<Object> obj(new Object(material));
  obj->quad_ = Quad::CreateFromRect(position, size, z);
  return obj;
}

std::unique_ptr<Object> Object::CreateCircle(const glm::vec2& position,
                                             float radius,
                                             float z,
                                             const Material* material) {
  // TODO(jeffbrown): make this less chunky, influence construction of
  // shader program
  return CreateRect(position, glm::vec2(radius, radius), z, material);
}

}  // namespace escher
