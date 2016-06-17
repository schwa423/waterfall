// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/base/macros.h"
#include "escher/renderer.h"
#include "samples/sample_util/SampleApplication.h"

#include <glm/glm.hpp>

class WaterfallApplication : public SampleApplication {
 public:
  // 360x640 is the logical size of the Nexus 5 screen.
  WaterfallApplication() : SampleApplication("Waterfall", 360, 640) {
    app_bar_material_.set_color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    canvas_material_.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    fab_material_.set_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  }

  bool initialize() override { return renderer_.Init(); }

  void step(float dt, double total_time) override {
    // TODO(abarth): We should convert total_time to TimePoint.
    frame_time_ = escher::Now();

    auto window = getWindow();
    window_size_ = escher::SizeI(window->getWidth(), window->getHeight());
    renderer_.SetSize(window_size_);
  }

  void draw() override {
    escher::Model model;

    // app bar
    model.AddObject(escher::Object::CreateRect(
        glm::vec2(0.0f, 0.0f), glm::vec2(window_size_.width(), 56.0f), 4.0f,
        &app_bar_material_));

    // canvas
    model.AddObject(escher::Object::CreateRect(
        glm::vec2(0.0f, 0.0f), window_size_.AsVec2(), 0.0f, &canvas_material_));

    // fab
    model.AddObject(escher::Object::CreateCircle(
        glm::vec2(window_size_.width() - 56.0f - 16.0,
                  window_size_.height() - 56.0f - 16.0),
        56.0f, 6.0f, &fab_material_));

    renderer_.Render(model);
  }

 private:
  escher::Material app_bar_material_;
  escher::Material canvas_material_;
  escher::Material fab_material_;

  escher::TimePoint frame_time_;
  escher::SizeI window_size_;
  escher::Renderer renderer_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(WaterfallApplication);
};

int main(int argc, const char* argv[]) {
  WaterfallApplication app;
  return app.run();
}
