// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "escher/gl/unique_program.h"

namespace escher {

class ShadowShader {
 public:
  ShadowShader();
  ~ShadowShader();

  bool Compile();

  const UniqueProgram& program() const { return program_; }

  GLint matrix() const { return matrix_; }
  GLint shadow_map() const { return shadow_map_; }
  GLint color() const { return color_; }

  GLint position() const { return position_; }

 private:
  UniqueProgram program_;

  GLint matrix_ = 0;
  GLint shadow_map_ = 0;
  GLint color_ = 0;
  GLint position_ = 0;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(ShadowShader);
};

}  // namespace escher
