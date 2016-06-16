// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/gl/depth_buffer.h"

#include <iostream>

namespace escher {

DepthBuffer::DepthBuffer() {
}

DepthBuffer::~DepthBuffer() {
}

bool DepthBuffer::SetSize(const SizeI& size) {
  if (size_.Equals(size))
    return !!texture_;

  if (!frame_buffer_)
    frame_buffer_.Generate();
  texture_.Generate();

  glBindTexture(GL_TEXTURE_2D, texture_.id());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.width(),
               size.height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);

  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_.id());
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         texture_.id(), 0);

#ifndef NDEBUG
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "error: frame buffer: " << std::hex << status << std::endl;
    return false;
  }
  return true;
#else
  return true;
#endif
}

}  // namespace escher
