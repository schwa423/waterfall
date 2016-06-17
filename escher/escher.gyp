# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [{
    'target_name': 'escher',
    'type': 'static_library',
    'dependencies': [
      '<(DEPTH)/samples/samples.gyp:sample_util'
    ],
    'includes': [ '../build/common_defines.gypi', ],
    'defines': [
      'GLM_FORCE_PURE=1',
    ],
    'include_dirs': [
      '..',
      '../third_party/glm',
    ],
    'sources': [
      'base/arraysize.h',
      'base/macros.h',
      'base/time.cc',
      'base/time.h',
      'geometry/quad.cc',
      'geometry/quad.h',
      'geometry/size_i.cc',
      'geometry/size_i.h',
      'gl/depth_buffer.cc',
      'gl/depth_buffer.h',
      'gl/extensions.cc',
      'gl/extensions.h',
      'gl/unique_frame_buffer.cc',
      'gl/unique_frame_buffer.h',
      'gl/unique_object.h',
      'gl/unique_program.cc',
      'gl/unique_program.h',
      'gl/unique_texture.cc',
      'gl/unique_texture.h',
      'scene/directional_light.cc',
      'scene/directional_light.h',
      'scene/dome_light.cc',
      'scene/dome_light.h',
      'scene/stage.cc',
      'scene/stage.h',
      'scene/viewing_volume.cc',
      'scene/viewing_volume.h',
      'shaders/depth_shader.cc',
      'shaders/depth_shader.h',
      'shaders/solid_color_shader.cc',
      'shaders/solid_color_shader.h',
      'renderer.cc',
      'renderer.h',
    ],
  }],
}