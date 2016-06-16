// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace escher {

void InitGLExtensions();

typedef void(GL_APIENTRYP PFNGLBINDUNIFORMLOCATIONCHROMIUMPROC)(
    GLuint mProgram, GLint location, const GLchar *name);

extern PFNGLBINDUNIFORMLOCATIONCHROMIUMPROC glBindUniformLocation;

}  // namespace escher
