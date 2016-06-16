// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace escher {

class UniqueTexture {
 public:
  UniqueTexture();
  ~UniqueTexture();

  GLuint id() const { return id_; }

  void Generate();
  void Reset();

  UniqueTexture(UniqueTexture&& other);
  UniqueTexture& operator=(UniqueTexture&& other);
  explicit operator bool() const { return id_ != 0; }

 private:
  GLuint id_ = 0;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(UniqueTexture);
};

}  // namespace escher
