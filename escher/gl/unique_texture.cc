// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/gl/unique_texture.h"

namespace escher {

UniqueTexture MakeUniqueTexture() {
  GLuint id = 0;
  glGenTextures(1, &id);
  UniqueTexture texture;
  texture.Reset(id);
  return texture;
}

}  // namespace escher
