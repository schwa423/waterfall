// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/lighting_filter.h"

namespace escher {
namespace {

constexpr char g_vertex_shader[] = R"GLSL(
  attribute vec3 a_position;
  varying vec2 fragment_uv;

  void main() {
    gl_Position = vec4(a_position, 1.0);
    fragment_uv = gl_Position.xy * 0.5 + 0.5;
  }
)GLSL";

constexpr char g_fragment_shader[] = R"GLSL(
  precision mediump float;
  uniform sampler2D u_illumination_map;
  uniform vec2 u_tap_stride;
  varying vec2 fragment_uv;

  const float scene_depth = 26.0;

  #define RADIUS 4

  void main() {
    vec4 center_tap = texture2D(u_illumination_map, fragment_uv);
    float center_key = center_tap.z * scene_depth;

    float sum = center_tap.x;
    float total_weight = 1.0;

    for (int r = RADIUS; r >= 1; --r) {
      vec2 tap_offset = float(-r) * u_tap_stride;
      vec4 tap = texture2D(u_illumination_map, fragment_uv + tap_offset);

      float tap_key = tap.z * scene_depth;
      float position_weight = float(RADIUS - r + 1) / float(RADIUS + 1);
      float tap_weight = position_weight * max(0.0, 1.0 - abs(tap_key - center_key));
      sum += tap_weight * tap.x;
      total_weight += tap_weight;
    }

    for (int r = 1; r <= RADIUS; ++r) {
      vec2 tap_offset = float(r) * u_tap_stride;
      vec4 tap = texture2D(u_illumination_map, fragment_uv + tap_offset);

      float tap_key = tap.z * scene_depth;
      float position_weight = float(RADIUS - r + 1) / float(RADIUS + 1);
      float tap_weight = position_weight * max(0.0, 1.0 - abs(tap_key - center_key));
      sum += tap_weight * tap.x;
      total_weight += tap_weight;
    }

    float illumination = sum / total_weight;
    gl_FragColor = vec4(illumination, 0.0, center_tap.z, 1.0);
  }
)GLSL";

}  // namespace

LightingFilter::LightingFilter() {}

LightingFilter::~LightingFilter() {}

bool LightingFilter::Compile() {
  program_ = MakeUniqueProgram(g_vertex_shader, g_fragment_shader);
  if (!program_)
    return false;
  illumination_map_ = glGetUniformLocation(program_.id(), "u_illumination_map");
  ESCHER_DCHECK(illumination_map_ != -1);
  tap_stride_ = glGetUniformLocation(program_.id(), "u_tap_stride");
  ESCHER_DCHECK(tap_stride_ != -1);
  return true;
}

}  // namespace escher
