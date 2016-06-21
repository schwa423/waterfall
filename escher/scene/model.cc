// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/scene/model.h"

#include <utility>

namespace escher {

Model::Model() {}

Model::~Model() {}

Model::Model(Model&& other) : objects_(std::move(other.objects_)) {}

Model& Model::operator=(Model&& other) {
  objects_ = std::move(other.objects_);
}

void Model::AddObject(std::unique_ptr<Object> obj) {
  objects_.emplace_back(std::move(obj));
}

}  // namespace escher
