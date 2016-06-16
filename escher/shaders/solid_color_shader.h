// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "util/Matrix.h"
#include "util/Vector.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace escher {

class SolidColorShader {
 public:
  SolidColorShader();
  ~SolidColorShader();

  bool Compile();

  GLuint program() const { return program_; }

  GLint matrix() const { return matrix_; }
  GLint color() const { return color_; }

  GLint position() const { return position_; }

 private:
  GLuint program_ = 0;

  GLint matrix_ = 0;
  GLint color_ = 0;
  GLint position_ = 0;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(SolidColorShader);
};

}  // namespace escher
