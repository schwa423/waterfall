// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>

#include "escher/base/macros.h"
#include "escher/base/time.h"
#include "escher/geometry/quad.h"
#include "escher/geometry/size_i.h"
#include "escher/gl/depth_buffer.h"
#include "escher/scene/stage.h"
#include "escher/scene/model.h"
#include "escher/shaders/depth_shader.h"
#include "escher/shaders/shadow_shader.h"
#include "escher/shaders/solid_color_shader.h"

namespace escher {

class Renderer {
 public:
  Renderer();
  ~Renderer();

  bool Init();

  void SetSize(SizeI size);
  void Render(const Model& model);

 private:
  void DrawModelWithDepthShader(const Model& model,
                                const glm::mat4& light_matrix);
  void DrawModelWithShadowShader(const Model& model,
                                 const glm::mat4& projection_matrix);

  Stage stage_;
  DepthBuffer shadow_map_;
  DepthShader depth_shader_;
  SolidColorShader solid_color_shader_;
  ShadowShader shadow_shader_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(Renderer);
};

}  // namespace escher
