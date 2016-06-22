// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <glm/glm.hpp>
#include <math.h>

#include "escher/base/macros.h"
#include "escher/renderer.h"
#include "samples/sample_util/SampleApplication.h"
#include "waterfall/scenes/app_test_scene.h"
#include "waterfall/scenes/material_stage.h"
#include "waterfall/scenes/shadow_test_scene.h"

namespace {

constexpr int kDefaultWindowWidth = 360;
constexpr int kDefaultWindowHeight = 640;
constexpr bool kDrawShadowTestScene = false;

}  // namespace

class WaterfallApplication : public SampleApplication {
 public:
  // 360x640 is the logical size of the Nexus 5 screen.
  WaterfallApplication()
      : SampleApplication("Waterfall",
                          kDefaultWindowWidth,
                          kDefaultWindowHeight) {}

  bool initialize() override {
    InitStageForMaterial(&stage_);
    return renderer_.Init();
  }

  void step(float dt, double total_time) override {
    auto window = getWindow();
    stage_.SetSize(escher::SizeI(window->getWidth(), window->getHeight()));

    Event event;
    while (window->popEvent(&event)) {
      if (event.Type == Event::EVENT_MOUSE_BUTTON_PRESSED) {
        mouse_is_down_ = true;
      } else if (event.Type == Event::EVENT_MOUSE_BUTTON_RELEASED) {
        mouse_is_down_ = false;
      } else if (event.Type == Event::EVENT_MOUSE_MOVED) {
        if (mouse_is_down_)
          focus_ = glm::vec2(event.MouseMove.X, event.MouseMove.Y);
      }
    }
  }

  void draw() override { renderer_.Render(stage_, GetModel()); }

 private:
  escher::Model GetModel() {
    if (kDrawShadowTestScene)
      return shadow_test_scene_.GetModel(stage_.size());
    return app_test_scene_.GetModel(stage_.size(), focus_);
  }

  escher::Stage stage_;
  escher::Renderer renderer_;

  AppTestScene app_test_scene_;
  ShadowTestScene shadow_test_scene_;

  bool mouse_is_down_ = false;
  glm::vec2 focus_ =
      glm::vec2(kDefaultWindowWidth / 2.0f, kDefaultWindowHeight / 2.0f);

  ESCHER_DISALLOW_COPY_AND_ASSIGN(WaterfallApplication);
};

int main(int argc, const char* argv[]) {
  WaterfallApplication app;
  return app.run();
}
