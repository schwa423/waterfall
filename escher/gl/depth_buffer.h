// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "escher/geometry/size_i.h"
#include "escher/gl/unique_frame_buffer.h"
#include "escher/gl/unique_texture.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace escher {

class DepthBuffer {
 public:
  DepthBuffer();
  ~DepthBuffer();

  SizeI size() const { return size_; }
  bool SetSize(const SizeI& size);

  const UniqueFrameBuffer& frame_buffer() const { return frame_buffer_; };
  const UniqueTexture& texture() const { return texture_; };

 private:
  UniqueFrameBuffer frame_buffer_;
  UniqueTexture texture_;
  SizeI size_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(DepthBuffer);
};

}  // namespace escher
