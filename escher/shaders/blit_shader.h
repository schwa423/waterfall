// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "escher/gl/unique_program.h"

#include "escher/gl/extensions.h"

namespace escher {

class BlitShader {
 public:
  BlitShader();
  ~BlitShader();

  bool Compile();

  const UniqueProgram& program() const { return program_; }

  GLint source() const { return source_; }

  GLint position() const { return position_; }

 private:
  UniqueProgram program_;

  GLint source_ = 0;
  GLint position_ = 0;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(BlitShader);
};

}  // namespace escher
