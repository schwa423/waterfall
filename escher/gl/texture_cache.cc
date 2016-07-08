// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/gl/texture_cache.h"

#include "ftl/logging.h"

namespace escher {

TextureCache::TextureCache() {}

TextureCache::~TextureCache() {}

Texture TextureCache::GetDepthTexture(const SizeI& size) {
  TextureDescriptor descriptor;
  descriptor.size = size;
  descriptor.factory = MakeDepthTexture;
  return GetTexture(descriptor);
}

Texture TextureCache::GetColorTexture(const SizeI& size) {
  TextureDescriptor descriptor;
  descriptor.size = size;
  descriptor.factory = MakeColorTexture;
  return GetTexture(descriptor);
}

Texture TextureCache::GetTexture(const TextureDescriptor& descriptor) {
  auto it = cache_.find(descriptor);
  if (it == cache_.end())
    return Texture::Make(descriptor);
  Texture texture(descriptor, std::move(it->second));
  cache_.erase(it);
  return texture;
}

void TextureCache::PutTexture(Texture texture) {
  FTL_DCHECK(texture);
  cache_.emplace(texture.descriptor(), texture.TakeUniqueTexture());
}

void TextureCache::Clear() {
  cache_.clear();
}

}  // namespace escher
