// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/material/material_shader.h"

#include "escher/shaders/glsl_generator.h"

namespace escher {
namespace {

constexpr char g_decls[] = R"GLSL(
  #define BINDING_NONE 0
  #define BINDING_CONSTANT 1
  #define MASK_NONE 0
  #define MASK_CIRCULAR 1
)GLSL";

constexpr char g_vertex_shader[] = R"GLSL(
  attribute vec3 a_position;
  uniform mat4 u_matrix;

#if MASK == MASK_CIRCULAR
  attribute vec2 a_mask_uv;
  varying vec2 v_mask_uv;
  void mask() {
    v_mask_uv = a_mask_uv;
  }
#else
  void mask() {}
#endif

  void main() {
    gl_Position = u_matrix * vec4(a_position, 1.0);
    mask();
  }
)GLSL";

constexpr char g_fragment_shader[] = R"GLSL(
  precision mediump float;

#if COLOR_BINDING == BINDING_NONE
  vec4 color() {
    return vec4(0.0, 0.0, 0.0, 1.0); // should alpha be 0?
  }
#elif COLOR_BINDING == BINDING_CONSTANT
  uniform vec4 u_color;
  vec4 color() {
    return u_color;
  }
#endif

#if MASK == MASK_CIRCULAR
  varying vec2 v_mask_uv;
  bool mask() {
    return v_mask_uv.x * v_mask_uv.x + v_mask_uv.y * v_mask_uv.y <= 1.0;
  }
#else
  bool mask() { return true; }
#endif

  void main() {
    if (!mask()) discard; // FIXME(jeffbrown): inefficient!
    gl_FragColor = color();
  }
)GLSL";

void DefineBindingSymbol(GLSLGenerator& generator,
                         const std::string& symbol,
                         BindingType binding_type) {
  switch (binding_type) {
    case BindingType::kNone:
      generator.DefineSymbol(symbol, "BINDING_NONE");
      break;
    case BindingType::kConstant:
      generator.DefineSymbol(symbol, "BINDING_CONSTANT");
      break;
  }
}

void DefineMaskSymbol(GLSLGenerator& generator, Modifier::Mask mask) {
  switch (mask) {
    case Modifier::Mask::kNone:
      generator.DefineSymbol("MASK", "MASK_NONE");
      break;
    case Modifier::Mask::kCircular:
      generator.DefineSymbol("MASK", "MASK_CIRCULAR");
      break;
  }
}

}  // namespace

MaterialShader::MaterialShader(const MaterialShaderDescriptor& descriptor)
    : descriptor_(descriptor) {}

MaterialShader::~MaterialShader() {}

void MaterialShader::Use(const glm::mat4& matrix) const {
  glUseProgram(program_.id());
  glEnableVertexAttribArray(position_);
  if (descriptor_.mask == Modifier::Mask::kCircular)
    glEnableVertexAttribArray(mask_uv_);
  glUniformMatrix4fv(matrix_, 1, GL_FALSE, &matrix[0][0]);
}

void MaterialShader::Bind(const Material& material,
                          const Modifier& modifier) const {
  if (descriptor_.color_binding_type == BindingType::kConstant) {
    const glm::vec4& color = material.color().constant_value();
    glUniform4fv(color_, 1, &color[0]);
  }
}

bool MaterialShader::Compile() {
  std::string prologue = GeneratePrologue();
  std::vector<std::string> vertex_shader_sources{g_decls, prologue,
                                                 g_vertex_shader};
  std::vector<std::string> fragment_shader_sources{g_decls, prologue,
                                                   g_fragment_shader};
  UniqueShader vertex_shader =
      MakeUniqueShader(GL_VERTEX_SHADER, vertex_shader_sources);
  UniqueShader fragment_shader =
      MakeUniqueShader(GL_FRAGMENT_SHADER, fragment_shader_sources);
  if (!vertex_shader || !fragment_shader)
    return false;

  program_ =
      MakeUniqueProgram(std::move(vertex_shader), std::move(fragment_shader));
  if (!program_)
    return false;

  matrix_ = glGetUniformLocation(program_.id(), "u_matrix");
  ESCHER_DCHECK(matrix_ != -1);

  if (descriptor_.color_binding_type == BindingType::kConstant) {
    color_ = glGetUniformLocation(program_.id(), "u_color");
    ESCHER_DCHECK(color_ != -1);
  }

  if (descriptor_.mask == Modifier::Mask::kCircular) {
    mask_uv_ = glGetAttribLocation(program_.id(), "a_mask_uv");
    ESCHER_DCHECK(mask_uv_ != -1);
  }
  return true;
}

std::string MaterialShader::GeneratePrologue() {
  GLSLGenerator generator;
  DefineBindingSymbol(generator, "COLOR_BINDING",
                      descriptor_.color_binding_type);
  DefineMaskSymbol(generator, descriptor_.mask);
  return generator.GenerateCode();
}

}  // namespace escher
