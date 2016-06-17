// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/shadow_shader.h"

#include "escher/shaders/uniforms.h"
#include "util/shader_utils.h"

namespace escher {
namespace {

constexpr char g_vertex_shader[] = SHADER_SOURCE(
  attribute vec3 a_position;
  uniform mat4 u_matrix;
  uniform mat4 u_light_matrix;
  varying vec4 v_light_position;

  void main() {
    vec4 homogeneous_position = vec4(a_position, 1.0);
    v_light_position = u_light_matrix * homogeneous_position;
    gl_Position = u_matrix * homogeneous_position;
  }
);

constexpr char g_fragment_shader[] = SHADER_SOURCE(
  precision mediump float;
  uniform vec4 u_color;
  uniform sampler2D u_shadow_map;
  varying vec4 v_light_position;

  const float bias = 0.005;

  void main() {
    // Change coordinate systems from [-1, 1] to [0, 1].
    vec3 light_uv = v_light_position.xyz * 0.5 + 0.5;
    float occulder_depth = texture2D(u_shadow_map, light_uv.xy).r;
    float fragment_depth = light_uv.z - bias;
    float shadow = fragment_depth > occulder_depth ? 0.5 : 1.0;
    gl_FragColor = vec4(shadow * u_color.rgb, u_color.a);
  }
);

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
    &light_matrix_,
    &color_,
    &shadow_map_,
  }, {
    "u_matrix",
    "u_light_matrix",
    "u_color",
    "u_shadow_map",
  });

  position_ = 0;
  return true;
}

}  // namespace escher
