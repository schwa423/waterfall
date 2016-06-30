// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "escher/geometry/size_i.h"
#include "escher/gl/texture_descriptor.h"
#include "escher/gl/unique_texture.h"

namespace escher {

class Texture {
 public:
  Texture();
  Texture(TextureDescriptor descriptor, UniqueTexture texture);
  ~Texture();

  Texture(Texture&& other);
  Texture& operator=(Texture&& other);

  static Texture Make(TextureDescriptor descriptor);

  const TextureDescriptor& descriptor() const { return descriptor_; };
  GLuint id() const { return texture_.id(); }

  explicit operator bool() const { return static_cast<bool>(texture_); }

  UniqueTexture TakeUniqueTexture() { return std::move(texture_); }

 private:
  TextureDescriptor descriptor_;
  UniqueTexture texture_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(Texture);
};

}  // namespace escher
