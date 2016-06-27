// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "waterfall/scenes/app_test_scene.h"

#include "escher/base/arraysize.h"
#include "escher/renderer.h"

namespace {

constexpr float kFabSize = 56.0f;

}  // namespace

AppTestScene::AppTestScene() {
  app_bar_material_.set_color(
      escher::MakeConstantBinding(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
  canvas_material_.set_color(
      escher::MakeConstantBinding(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
  card_material_.set_color(
      escher::MakeConstantBinding(glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
  card_material_.set_displacement(escher::Displacement::MakeWave(
      glm::vec2(0.5f, 0.0f), glm::vec2(0.75f, 0.0f), 10.0f, -M_PI, 0.0f));
  fab_material_.set_color(
      escher::MakeConstantBinding(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
  green_material_.set_color(
      escher::MakeConstantBinding(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
}

AppTestScene::~AppTestScene() {}

escher::Model AppTestScene::GetModel(const escher::SizeI& size,
                                     const glm::vec2& focus) {
  std::vector<escher::Object> objects;

  // canvas
  objects.emplace_back(
      escher::Shape::CreateRect(glm::vec2(0.0f, 0.0f), size.AsVec2(), 0.0f),
      &canvas_material_);

  // app bar
  objects.emplace_back(
      escher::Shape::CreateRect(glm::vec2(0.0f, 0.0f),
                                glm::vec2(size.width(), 56.0f), 4.0f),
      &app_bar_material_);

  // card
  objects.emplace_back(
      escher::Shape::CreateRect(glm::vec2(0.0f, 200.0f),
                                glm::vec2(size.width(), 120.0f), 2.0f),
      &card_material_);

  // left eye
  objects.emplace_back(
      escher::Shape::CreateRect(glm::vec2(100.0f, 180.0f),
                                glm::vec2(60.0f, 40.0f), 10.0f),
      &green_material_);

  // right eye
  objects.emplace_back(
      escher::Shape::CreateRect(glm::vec2(200.0f, 180.0f),
                                glm::vec2(60.0f, 40.0f), 16.0f),
      &green_material_);

  // null
  objects.emplace_back(escher::Shape::CreateRect(glm::vec2(20.0f, 290.0f),
                                                 glm::vec2(40.0f, 40.0f), 2.0f),
                       &null_material_);

  // fab
  objects.emplace_back(
      escher::Shape::CreateCircle(
          glm::vec2(focus.x - kFabSize / 2.0f, focus.y - kFabSize / 2.0f),
          kFabSize / 2.0f, 6.0f),
      &fab_material_);

  return escher::Model(std::move(objects));
}
