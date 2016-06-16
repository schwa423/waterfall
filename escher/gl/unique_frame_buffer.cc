// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/gl/unique_frame_buffer.h"

#include "escher/base/macros.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <utility>

namespace escher {

UniqueFrameBuffer::UniqueFrameBuffer() {
}

UniqueFrameBuffer::~UniqueFrameBuffer() {
  Reset();
}

void UniqueFrameBuffer::Generate() {
  Reset();
  glGenFramebuffers(1, &id_);
}

void UniqueFrameBuffer::Reset() {
  if (id_) {
    GLuint id = id_;
    id_ = 0;
    glDeleteFramebuffers(1, &id);
  }
}

UniqueFrameBuffer::UniqueFrameBuffer(UniqueFrameBuffer&& other)
  : id_(other.id_) {
  other.id_ = 0;
}

UniqueFrameBuffer& UniqueFrameBuffer::operator=(UniqueFrameBuffer&& other) {
  std::swap(id_, other.id_);
  return *this;
}

}  // namespace escher
