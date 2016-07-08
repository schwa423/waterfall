// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ftl/time/time.h"

namespace ftl {

TimePoint Now() {
  return std::chrono::steady_clock::now();
}

}  // namespace ftl
