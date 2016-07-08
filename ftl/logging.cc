// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>

#include "ftl/debug/debugger.h"
#include "ftl/logging.h"

namespace ftl {

LogMessage::LogMessage(LogSeverity severity,
                       const char* file,
                       int line,
                       const char* condition)
  : severity_(severity), file_(file), line_(line) {
  stream_ << "Check failed: " << condition << ". ";
}

LogMessage::~LogMessage() {
  stream_ << std::endl;
  std::cerr << stream_.str();
  std::cerr.flush();

  if (severity_ >= LOG_FATAL)
    BreakDebugger();
}

}  // namespace ftl
