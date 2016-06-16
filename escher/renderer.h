// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "escher/base/macros.h"
#include "escher/base/time.h"
#include "escher/geometry/quad.h"
#include "escher/geometry/size_i.h"
#include "escher/scene/stage.h"
#include "escher/shaders/solid_color_shader.h"

namespace escher {

class Renderer {
 public:
  Renderer();
  ~Renderer();

  bool Init();

  void SetSize(SizeI size);
  void Render(TimePoint frame_time);

 private:
  void DrawSolidColorQuad(const Quad& quad, const Vector4& color);

  Stage stage_;
  SolidColorShader solid_color_shader_;
  Quad app_bar_;
  Quad canvas_;
  Quad fab_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(Renderer);
};

}  // namespace escher
