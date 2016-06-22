// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <glm/glm.hpp>

#include "escher/base/macros.h"
#include "escher/gl/unique_program.h"
#include "escher/scene/material.h"
#include "escher/shaders/material/material_shader_descriptor.h"
#include "escher/shaders/material/modifier.h"

namespace escher {

// Shader for materials.
// Generally responsible for writing color and depth to the framebuffer.
class MaterialShader {
 public:
  ~MaterialShader();

  // Attributes
  GLint position() const { return position_; }
  GLint mask_uv() const { return mask_uv_; }

  // Prepares the program for use before drawing any number of objects.
  // Must be called whenever switching between shaders and before binding
  // a material.
  void Use(const glm::mat4& matrix) const;

  // Binds the parameters of a material to the shader.
  void Bind(const Material& material, const Modifier& modifier) const;

 private:
  friend class MaterialShaderFactory;

  explicit MaterialShader(const MaterialShaderDescriptor& descriptor);
  bool Compile();
  std::string GeneratePrologue();

  const MaterialShaderDescriptor descriptor_;

  UniqueProgram program_;
  GLint matrix_ = 0;
  GLint color_ = 0;
  GLint position_ = 0;
  GLint mask_uv_ = 0;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(MaterialShader);
};

}  // namespace escher
