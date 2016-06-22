// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <glm/glm.hpp>
#include <math.h>

#include "escher/base/macros.h"
#include "escher/renderer.h"
#include "samples/sample_util/SampleApplication.h"
#include "waterfall/shadow_test_scene.h"

namespace {

constexpr float kFabSize = 56.0f;
constexpr int kDefaultWindowWidth = 360;
constexpr int kDefaultWindowHeight = 640;

// Material design places objects from 0.0f to 24.0f. We inflate that range
// slightly to avoid clipping at the edges of the viewing volume.
constexpr float kNear = 25.0f;
constexpr float kFar = -1.0f;

constexpr bool kDrawShadowTestScene = false;

}  // namespace

class WaterfallApplication : public SampleApplication {
 public:
  // 360x640 is the logical size of the Nexus 5 screen.
  WaterfallApplication()
      : SampleApplication("Waterfall",
                          kDefaultWindowWidth,
                          kDefaultWindowHeight) {
    app_bar_material_.set_color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    canvas_material_.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    card_material_.set_color(glm::vec4(1.0f, 1.0f, 0.8f, 1.0f));
    fab_material_.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    green_material_.set_color(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  }

  bool initialize() override {
    stage_.set_key_light(escher::DirectionalLight(
        glm::vec2(M_PI / 2.0, M_PI / 4.0), M_PI / 4.0, 0.5));
    return renderer_.Init();
  }

  void step(float dt, double total_time) override {
    // TODO(abarth): We should convert total_time to TimePoint.
    frame_time_ = escher::Now();

    auto window = getWindow();
    auto size = escher::SizeI(window->getWidth(), window->getHeight());
    stage_.set_viewing_volume(escher::ViewingVolume(size, kNear, kFar));

    Event event;
    while (window->popEvent(&event)) {
      if (event.Type == Event::EVENT_MOUSE_BUTTON_PRESSED) {
        mouse_is_down_ = true;
      } else if (event.Type == Event::EVENT_MOUSE_BUTTON_RELEASED) {
        mouse_is_down_ = false;
      } else if (event.Type == Event::EVENT_MOUSE_MOVED) {
        if (mouse_is_down_) {
          fab_x_ = event.MouseMove.X;
          fab_y_ = event.MouseMove.Y;
        }
      }
    }
  }

  void draw() override {
    if (kDrawShadowTestScene) {
      escher::Model model = shadow_test_scene_.GetModel(stage_.size());
      renderer_.Render(stage_, model);
      return;
    }
    std::vector<std::unique_ptr<escher::Object>> objects;

    // app bar
    objects.emplace_back(escher::Object::CreateRect(
        glm::vec2(0.0f, 0.0f), glm::vec2(stage_.size().width(), 56.0f), 4.0f,
        &app_bar_material_));

    objects.emplace_back(escher::Object::CreateRect(glm::vec2(100.0f, 180.0f),
                                               glm::vec2(60.0f, 40.0f), 10.0f,
                                               &green_material_));
    objects.emplace_back(escher::Object::CreateRect(glm::vec2(200.0f, 180.0f),
                                               glm::vec2(60.0f, 40.0f), 16.0f,
                                               &green_material_));
    objects.emplace_back(escher::Object::CreateRect(
        glm::vec2(0.0f, 200.0f), glm::vec2(stage_.size().width(), 80.0f), 2.0f,
        &card_material_));

    // canvas
    objects.emplace_back(escher::Object::CreateRect(
        glm::vec2(0.0f, 0.0f), stage_.size().AsVec2(), 0.0f, &canvas_material_));

    // fab
    objects.emplace_back(escher::Object::CreateCircle(
        glm::vec2(fab_x_ - kFabSize / 2.0f, fab_y_ - kFabSize / 2.0f), kFabSize,
        6.0f, &fab_material_));

    renderer_.Render(stage_, escher::Model(std::move(objects)));
  }

 private:
  escher::Renderer renderer_;
  escher::Stage stage_;
  escher::TimePoint frame_time_;

  ShadowTestScene shadow_test_scene_;

  escher::Material app_bar_material_;
  escher::Material canvas_material_;
  escher::Material card_material_;
  escher::Material fab_material_;
  escher::Material green_material_;

  bool mouse_is_down_ = false;
  float fab_x_ = kDefaultWindowWidth - kFabSize / 2.0f - 16.0f;
  float fab_y_ = kDefaultWindowHeight - kFabSize / 2.0f - 16.0f;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(WaterfallApplication);
};

int main(int argc, const char* argv[]) {
  WaterfallApplication app;
  return app.run();
}
