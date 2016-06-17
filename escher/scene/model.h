// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include <vector>

#include "escher/scene/object.h"

namespace escher {

class Model {
 public:
  Model();
  ~Model();

  void AddObject(std::unique_ptr<Object> obj);
  const std::vector<std::unique_ptr<Object>>& objects() const {
    return objects_;
  }

 private:
  std::vector<std::unique_ptr<Object>> objects_;
};

}  // namespace escher
