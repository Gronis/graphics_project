#pragma once
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <thirdparty/ecs.h>

#include "util/Manager.h"
#include "Renderer.h"
#include "Gbuffer.h"

namespace engine {
namespace gfx {
class Window {

 public:
  Window(const char *title, int width, int height, bool use_virtual_sync, bool fullscreen);
  Window(const char *title, int width, int height) : Window(title, width, height, true, false) { }
  ~Window();

  void close();
  void update(double dt);
  void swap_buffers();

  inline int x() { return x_; }
  inline int y() { return y_; }
  inline int width() { return width_; }
  inline int height() { return height_; }
  inline float ratio() { return width() / (float) height(); }
  inline bool closed() { return closed_; }
  inline bool focused() { return focused_; }
  inline glm::mat4 camera() { return camera_; }
  inline void camera(glm::mat4 camera) { camera_ = camera; }
  inline glm::vec3 camera_position() { return camera_position_; }
  inline glm::mat4 projection() { return projection_; }
  inline void projection(glm::mat4 projection) { projection_ = projection; }
  inline Gbuffer& gbuffer() { return gbuffer_; }

 private:

  void handle_input(float dt);

  static void resize_(GLFWwindow* window, int width, int height);
  void resize(int width, int height);

  static void focus_(GLFWwindow* window, int focused);
  void focus(bool focused);


  int y_;
  int x_;
  int width_;
  int height_;
  bool focused_ = true;
  bool closed_ = false;

  bool use_virtual_sync_;
  bool fullscreen_;

  double x_pos_last_;
  double y_pos_last_;

  glm::vec3 camera_position_;
  glm::vec3 camera_position_pref;
  glm::vec3 camera_direction_;
  glm::vec3 camera_direction_pref;

  glm::mat4 camera_;
  glm::mat4 projection_;
  Gbuffer gbuffer_;


  GLFWwindow *window_;
};
}
}
