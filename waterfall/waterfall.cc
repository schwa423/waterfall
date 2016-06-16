// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "escher/macros.h"
#include "escher/renderer.h"
#include "samples/sample_util/SampleApplication.h"
#include "util/geometry_utils.h"
#include "util/Matrix.h"
#include "util/shader_utils.h"
#include "util/Vector.h"

#include <cmath>

constexpr char g_vertex_shader[] = SHADER_SOURCE(
  uniform mat4 u_matrix;
  attribute vec4 a_position;
  attribute vec2 a_texture_coord;
  varying vec2 v_texture_coord;

  void main() {
    gl_Position = u_matrix * a_position;
    v_texture_coord = a_texture_coord;
  }
);

constexpr char g_fragment_shader[] = SHADER_SOURCE(
  precision mediump float;
  varying vec2 v_texture_coord;

  void main() {
    gl_FragColor = vec4(v_texture_coord.x, v_texture_coord.y, 1.0, 1.0);
  }
);

class WaterfallApplication : public SampleApplication {
 public:
  WaterfallApplication() : SampleApplication("Waterfall", 1280, 720) {}

  bool initialize() override {
    program_ = CompileProgram(g_vertex_shader, g_fragment_shader);
    if (!program_)
      return false;

    position_location_ = glGetAttribLocation(program_, "a_position");
    texture_coord_location_ = glGetAttribLocation(program_, "a_texture_coord");
    matrix_location_ = glGetUniformLocation(program_, "u_matrix");

    GenerateCubeGeometry(0.5f, &cube_);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    return true;
  }

  void destroy() override {
    glDeleteProgram(program_);
  }

  void step(float dt, double total_time) override {
    angle_ = fmod(angle_ + (dt * 40.0f), 360.0f);

    auto window = getWindow();
    float aspect_ratio = float(window->getWidth()) / window->getHeight();
    Matrix4 matrix =
        Matrix4::perspective(60.0f, aspect_ratio, 1.0f, 20.0f) *
        Matrix4::translate(Vector3(0.0f, 0.0f, -2.0f)) *
        Matrix4::rotate(angle_, Vector3(1.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(matrix_location_, 1, GL_FALSE, matrix.data);
  }

  void draw() override {
    auto window = getWindow();
    glViewport(0, 0, window->getWidth(), window->getHeight());
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program_);
    glVertexAttribPointer(position_location_, 3, GL_FLOAT, GL_FALSE, 0,
                          cube_.positions.data());
    glEnableVertexAttribArray(position_location_);

    glVertexAttribPointer(texture_coord_location_, 2, GL_FLOAT, GL_FALSE, 0,
                          cube_.texcoords.data());
    glEnableVertexAttribArray(texture_coord_location_);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cube_.indices.size()),
                   GL_UNSIGNED_SHORT, cube_.indices.data());
  }

 private:
  GLuint program_ = 0;
  GLint position_location_ = 0;
  GLint texture_coord_location_ = 0;
  GLuint matrix_location_ = 0;
  float angle_ = 45.0f;
  CubeGeometry cube_;

  ESCHER_DISALLOW_COPY_AND_ASSIGN(WaterfallApplication);
};

int main(int argc, const char* argv[]) {
  WaterfallApplication app;
  return app.run();
}
