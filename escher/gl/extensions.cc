// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/gl/extensions.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace escher {
namespace {

bool g_initialized = false;

}  // namespace

PFNGLBINDUNIFORMLOCATIONCHROMIUMPROC glBindUniformLocation = nullptr;

void InitGLExtensions() {
  if (g_initialized)
    return;

  glBindUniformLocation = reinterpret_cast<PFNGLBINDUNIFORMLOCATIONCHROMIUMPROC>(
      eglGetProcAddress("glBindUniformLocationCHROMIUM"));

  g_initialized = true;
}

}  // namespace escher
