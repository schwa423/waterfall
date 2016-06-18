// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/shadow_shader.h"

#include "escher/shaders/uniforms.h"
#include "util/shader_utils.h"

namespace escher {
namespace {

constexpr char g_vertex_shader[] = R"GLSL(
  attribute vec3 a_position;
  uniform mat4 u_matrix;
  varying vec4 v_shadow_position;

  void main() {
    gl_Position = v_shadow_position =
        u_matrix * vec4(a_position, 1.0);
  }
)GLSL";

constexpr char g_fragment_shader[] = R"GLSL(
  precision mediump float;
  uniform vec4 u_color;
  uniform sampler2D u_shadow_map;
  varying vec4 v_shadow_position;

  const float bias = 0.005;

  void main() {
    // Change coordinate systems from [-1, 1] to [0, 1].
    vec2 shadow_uv = v_shadow_position.xy * 0.5 + 0.5;
    float occluder_depth = texture2D(u_shadow_map, shadow_uv).r;
    float fragment_depth = v_shadow_position.z - bias;
    float shadow = fragment_depth > occluder_depth ? 0.5 : 1.0;
    gl_FragColor = vec4(shadow * u_color.rgb, u_color.a);
  }
)GLSL";

}  // namespace

ShadowShader::ShadowShader() {
}

ShadowShader::~ShadowShader() {
}

bool ShadowShader::Compile() {
  program_ = MakeUniqueProgram(g_vertex_shader, g_fragment_shader);
  if (!program_)
    return false;

  BindUniforms(program_.id(), {
    &matrix_,
    &color_,
    &shadow_map_,
  }, {
    "u_matrix",
    "u_color",
    "u_shadow_map",
  });

  position_ = 0;
  return true;
}

}  // namespace escher
