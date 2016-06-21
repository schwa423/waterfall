# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [{
    'target_name': 'waterfall',
    'type': 'executable',
    'dependencies': [
      '<(DEPTH)/samples/samples.gyp:sample_util',
      '<(DEPTH)/escher/escher.gyp:escher',
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
      'shadow_test_scene.cc',
      'shadow_test_scene.h',
      'waterfall.cc',
    ],
  }],
}
