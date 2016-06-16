// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "escher/scene/viewing_volume.h"

#include <utility>

namespace escher {

class Stage {
 public:
  Stage();
  ~Stage();

  const ViewingVolume& viewing_volume() const {
    return viewing_volume_;
  };
  void set_viewing_volume(ViewingVolume value) {
    viewing_volume_ = std::move(value);
  }

  const SizeI& size() const { return viewing_volume_.size(); }

 private:
  ViewingVolume viewing_volume_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(Stage);
};

}  // namespace escher
