// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "ftl/macros.h"
#include "escher/scene/model.h"
#include "escher/scene/viewing_volume.h"

class ShadowTestScene {
 public:
  ShadowTestScene();
  ~ShadowTestScene();

  escher::Model GetModel(const escher::ViewingVolume& volume);

 private:
  escher::Material card_material_;

  FTL_DISALLOW_COPY_AND_ASSIGN(ShadowTestScene);
};
