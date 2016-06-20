// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>

#include "escher/base/macros.h"
#include "escher/base/time.h"
#include "escher/geometry/quad.h"
#include "escher/geometry/size_i.h"
#include "escher/gl/frame_buffer.h"
#include "escher/scene/stage.h"
#include "escher/scene/model.h"
#include "escher/shaders/blit_shader.h"
#include "escher/shaders/depth_shader.h"
#include "escher/shaders/illumination_shader.h"
#include "escher/shaders/lighting_filter.h"
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
  void Blit(GLuint texture_id);
  void DrawModelWithSolidColorShader(const Model& model,
                                     const glm::mat4& matrix);
  // void DrawModelWithDepthShader(const Model& model, const glm::mat4& matrix);
  // void DrawModelWithShadowShader(const Model& model, const glm::mat4&
  // matrix);

  void ComputeIllumination();
  void DrawFullFrameQuad(GLint position);

  Stage stage_;
  FrameBuffer scene_buffer_;
  FrameBuffer lighting_buffer_;
  UniqueTexture scratch_texture_;
  Quad full_frame_;

  BlitShader blit_shader_;
  DepthShader depth_shader_;
  IlluminationShader illumination_shader_;
  LightingFilter lighting_filter_;
  ShadowShader shadow_shader_;
  SolidColorShader solid_color_shader_;

  UniqueTexture noise_texture_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(Renderer);
};

}  // namespace escher
