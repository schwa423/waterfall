// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/base/macros.h"
#include "escher/renderer.h"
#include "samples/sample_util/SampleApplication.h"

class WaterfallApplication : public SampleApplication {
 public:
  // 360x640 is the logical size of the Nexus 5 screen.
  WaterfallApplication() : SampleApplication("Waterfall", 360, 640) {}

  bool initialize() override {
    return renderer_.Init();
  }

  void step(float dt, double total_time) override {
    // TODO(abarth): We should convert total_time to TimePoint.
    frame_time_ = escher::Now();

    auto window = getWindow();
    renderer_.SetSize(escher::SizeI(window->getWidth(), window->getHeight()));
  }

  void draw() override {
    renderer_.Render(frame_time_);
  }

 private:
  escher::TimePoint frame_time_;
  escher::Renderer renderer_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(WaterfallApplication);
};

int main(int argc, const char* argv[]) {
  WaterfallApplication app;
  return app.run();
}
