// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/gl/unique_object.h"

namespace escher {
namespace internal {

inline void DeleteTexture(GLuint id) {
  glDeleteTextures(1, &id);
}

}  // internal

typedef UniqueObject<internal::DeleteTexture> UniqueTexture;
UniqueTexture MakeUniqueTexture();

}  // namespace escher