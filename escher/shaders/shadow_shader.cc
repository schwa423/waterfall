// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/shadow_shader.h"

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
  uniform sampler2D u_depth_map;
  uniform sampler2D u_noise;
  varying vec2 fragment_uv;

  const vec2 view_size = vec2(360.0, 640.0);
  const float scene_depth = 26.0;
  const int kTapCount = 8;
  const int kSpiralTurnCount = 7;
  const float kLightRadiusDP = 16.0;

  // Must match header.
  const int kNoiseSize = 5;

  float sampleOcclusion(vec2 fragment_uv,
                        vec3 fragment_dp,
                        int index,
                        float theta0) {
    float alpha = (float(index) + 0.5) / float(kTapCount);
    float theta = alpha * float(kSpiralTurnCount) * 6.28 + theta0;
    vec2 tap_delta_dp = alpha * kLightRadiusDP * vec2(cos(theta), abs(sin(theta)));
    vec2 tap_delta_uv = tap_delta_dp / view_size;
    float tap_depth_uv = texture2D(u_depth_map, fragment_uv + tap_delta_uv).r;
    float tap_depth_dp = tap_depth_uv * scene_depth;
    float depth_delta_dp = tap_depth_dp - fragment_dp.z;
    float occlusion_z = -tap_delta_dp.y * tan(3.14 / 4.0);
    return float(depth_delta_dp < occlusion_z);
  }

  void main() {
    float seed = texture2D(u_noise, gl_FragCoord.xy / float(kNoiseSize)).r;
    float theta0 = 6.28 * seed;
    float fragment_depth_uv = texture2D(u_depth_map, fragment_uv).r;
    vec3 fragment_dp = vec3(fragment_uv * view_size, fragment_depth_uv * scene_depth);

    float occlusion = 0.0;
    for (int i = 0; i < kTapCount; ++i)
      occlusion += sampleOcclusion(fragment_uv, fragment_dp, i, theta0);
    occlusion = clamp(occlusion / float(kTapCount), 0.0, 1.0);
    gl_FragColor = vec4(occlusion, 0.0, fragment_depth_uv, 1.0);
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
  depth_map_ = glGetUniformLocation(program_.id(), "u_depth_map");
  ESCHER_DCHECK(depth_map_ != -1);
  noise_ = glGetUniformLocation(program_.id(), "u_noise");
  ESCHER_DCHECK(noise_ != -1);
  return true;
}

}  // namespace escher
