// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/base/time.h"

namespace escher {

TimePoint Now() {
  return std::chrono::steady_clock::now();
}

}  // namespace escher
