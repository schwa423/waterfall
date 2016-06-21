// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/base/macros.h"
#include "escher/renderer.h"

class ShadowTestScene {
 public:
  ShadowTestScene();
  ~ShadowTestScene();

  escher::Model GetModel(const escher::SizeI& size);

 private:
  escher::Material card_material_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(ShadowTestScene);
};
