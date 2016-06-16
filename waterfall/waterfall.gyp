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
    'include_dirs': ['..'],
    'sources': [ 
      'waterfall.cc',
    ],
  }],
}
