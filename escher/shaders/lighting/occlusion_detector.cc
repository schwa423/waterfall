// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/shaders/lighting/occlusion_detector.h"

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
  uniform vec3 u_viewing_volume;
  varying vec2 fragment_uv;

  const float kPi = 3.14159265359;

  // Must match header.
  const int kNoiseSize = 5;

  // The numer of screen-space samples to use in the computation.
  const int kTapCount = 8;

  // These should be relatively primary to each other and to kTapCount;
  const vec2 kSpirals = vec2(7.0, 5.0);

  const float kSampleHemisphereRadius = 16.0;  // screen pixels.

  // Terminology:
  //
  //  * theta is the azimuthal angle
  //  * phi for the polar angle

  // The light's primary direction in polar coordinates, (theta, phi). For
  // simplicity, we assume that the light is infinitely far away (i.e., the
  // light is directional from this direction).
  const vec2 kLight = vec2(kPi / 2.0, kPi / 4.0);

  // Along the light is directional, the light has some amount of angular
  // dispersion (i.e., the light is not fully columnated). For simplicity, we
  // assume the dispersion of the light source is symmetric about the light's
  // primary direction.
  const float kLightDispersion = kPi / 4.0;
  const vec2 kLight0 = kLight - kLightDispersion / 2.0;

  const float kKeyLightIntensity = 0.5;
  const float kFillLightIntensity = 1.0 - kKeyLightIntensity;

  float sampleKeyIllumination(vec2 fragment_uv,
                              float fragment_z,
                              float alpha,
                              vec2 seed) {
    vec2 polar = kLight0 + fract(seed + alpha * kSpirals) * kLightDispersion;
    float theta = polar.x;
    float phi = polar.y;
    float radius = alpha * kSampleHemisphereRadius;

    vec2 tap_delta_uv = radius * sin(phi) * vec2(cos(theta), sin(theta)) / u_viewing_volume.xy;
    float tap_depth_uv = texture2D(u_depth_map, fragment_uv + tap_delta_uv).r;
    float tap_z = tap_depth_uv * -u_viewing_volume.z;

    float z = fragment_z + radius * abs(cos(phi));
    return float(z > tap_z);
  }

  float sampleFillIllumination(vec2 fragment_uv,
                               float fragment_z,
                               float alpha,
                               vec2 seed) {
    vec2 polar = 2.0 * kPi * (seed + alpha * kSpirals);
    float theta = polar.x;
    float phi = polar.y;
    float radius = alpha * kSampleHemisphereRadius;

    vec2 tap_delta_uv = radius * sin(phi) * vec2(cos(theta), sin(theta)) / u_viewing_volume.xy;
    float tap_depth_uv = texture2D(u_depth_map, fragment_uv + tap_delta_uv).r;
    float tap_z = tap_depth_uv * -u_viewing_volume.z;

    float z = fragment_z + radius * abs(cos(phi));
    return float(z > tap_z);
  }

  void main() {
    vec2 seed = texture2D(u_noise, gl_FragCoord.xy / float(kNoiseSize)).rg;

    float fragment_depth_uv = texture2D(u_depth_map, fragment_uv).r;
    float fragment_z = fragment_depth_uv * -u_viewing_volume.z;

    float L = 0.0;
    for (int i = 0; i < kTapCount; ++i) {
      float alpha = (float(i) + 0.5) / float(kTapCount);
      L += kKeyLightIntensity * sampleKeyIllumination(fragment_uv, fragment_z, alpha, seed);
      L += kFillLightIntensity * sampleFillIllumination(fragment_uv, fragment_z, alpha, seed);
    }
    L = clamp(L / float(kTapCount), 0.0, 1.0);
    gl_FragColor = vec4(L, 0.0, fragment_depth_uv, 1.0);
  }
)GLSL";

}  // namespace

OcclusionDetector::OcclusionDetector() {
}

OcclusionDetector::~OcclusionDetector() {
}

bool OcclusionDetector::Compile() {
  program_ = MakeUniqueProgram(g_vertex_shader, g_fragment_shader);
  if (!program_)
    return false;
  depth_map_ = glGetUniformLocation(program_.id(), "u_depth_map");
  ESCHER_DCHECK(depth_map_ != -1);
  noise_ = glGetUniformLocation(program_.id(), "u_noise");
  ESCHER_DCHECK(noise_ != -1);
  viewing_volume_ = glGetUniformLocation(program_.id(), "u_viewing_volume");
  ESCHER_DCHECK(viewing_volume_ != -1);
  return true;
}

}  // namespace escher
