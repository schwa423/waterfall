// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "escher/gl/unique_program.h"

#include "escher/gl/extensions.h"

namespace escher {

class LightingFilter {
 public:
  LightingFilter();
  ~LightingFilter();

  bool Compile();

  const UniqueProgram& program() const { return program_; }

  GLint occlusions() const { return occlusions_; }
  GLint tap_stride() const { return tap_stride_; }

  GLint position() const { return position_; }

 private:
  UniqueProgram program_;

  GLint occlusions_ = 0;
  GLint tap_stride_ = 0;
  GLint position_ = 0;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(LightingFilter);
};

}  // namespace escher
