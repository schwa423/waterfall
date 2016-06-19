// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/shadow_shader.h"

#include "escher/shaders/uniforms.h"

namespace escher {
namespace {

constexpr char g_vertex_shader[] = R"GLSL(
  attribute vec3 a_position;
  uniform mat4 u_matrix;
  varying vec2 fragment_uv;

  void main() {
    gl_Position = u_matrix * vec4(a_position, 1.0);
    fragment_uv = gl_Position.xy * 0.5 + 0.5;
  }
)GLSL";

constexpr char g_fragment_shader[] = R"GLSL(
  precision mediump float;
  uniform vec4 u_color;
  uniform vec3 u_viewing_volume;
  uniform sampler2D u_depth_map;
  varying vec2 fragment_uv;

  const int kSampleCount = 8;
  const int kSpiralTurnCount = 7;
  const float kLightRadiusWS = 16.0;

  float sampleOcclusion(vec2 fragment_uv,
                        vec3 fragment_ws,
                        int index,
                        float theta0) {
    float alpha = (float(index) + 0.5) / float(kSampleCount);
    float theta = alpha * float(kSpiralTurnCount) * 6.28 + theta0;

    vec2 sample_unit = vec2(cos(theta) / u_viewing_volume.x,
                            sin(theta) / u_viewing_volume.y);
    vec2 sample_uv = fragment_uv + alpha * kLightRadiusWS * sample_unit;
    float sample_depth = texture2D(u_depth_map, sample_uv).r;

    vec3 sample_ws = u_viewing_volume * vec3(sample_uv, sample_depth);

    vec3 v = sample_ws - fragment_ws;
    float vv = dot(v, v);
    float vl = v.y;
    float vn = -v.z;
    const float radius2 = kLightRadiusWS * kLightRadiusWS;

    float clip = clamp(radius2 - vv, 0.0, 1.0);
    float depth_falloff = float(vn > 0.0) * vn * vn / vv;
    return clip * 2.0 * float(vl > 0.0) * depth_falloff;
  }

  float rand(vec2 seed){
    return fract(sin(dot(seed.xy, vec2(10.0429, 76.0125))) * 59491.0853);
  }

  void main() {
    float theta0 = rand(fragment_uv) * 3.14159 * 2.0;
    vec3 fragment_ws = u_viewing_volume * vec3(fragment_uv, gl_FragCoord.z);

    float occlusion = 0.0;
    for (int i = 0; i < kSampleCount; ++i)
      occlusion += sampleOcclusion(fragment_uv, fragment_ws, i, theta0);
    occlusion = clamp(occlusion / float(kSampleCount), 0.0, 1.0);

    float lighting = 1.0 - occlusion;
    gl_FragColor = vec4(lighting * u_color.xyz, u_color.a);
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
    &viewing_volume_,
    &depth_map_,
  }, {
    "u_matrix",
    "u_color",
    "u_viewing_volume",
    "u_depth_map",
  });

  position_ = 0;
  return true;
}

}  // namespace escher
