// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "escher/base/macros.h"
#include "escher/geometry/quad.h"
#include "escher/scene/material.h"

namespace escher {

class Object {
 public:
  ~Object();

  static std::unique_ptr<Object> CreateRect(const glm::vec2& position,
                                            const glm::vec2& size,
                                            float z,
                                            const Material* material);
  static std::unique_ptr<Object> CreateCircle(const glm::vec2& position,
                                              float radius,
                                              float z,
                                              const Material* material);

  const Material* material() const { return material_; }

  const float* positions() const { return quad_.data(); }
  const GLushort* indices() const { return Quad::GetIndices(); }
  GLsizei index_count() const { return Quad::GetIndexCount(); }

 private:
  Object(const Material* material);

  const Material* material_;
  Quad quad_;  // TODO(jeffbrown): generalize to arbitrary mesh with attributes

  ESCHER_DISALLOW_COPY_AND_ASSIGN(Object);
};

}  // namespace escher
