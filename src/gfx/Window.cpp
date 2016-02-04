#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Engine.h"
#include "Components.h"
#include "gfx/Texture.h"

using namespace engine::gfx;

Window::Window(const char *title, int width, int height, bool use_virtual_sync, bool fullscreen) {
  //init glfw
  static bool glfw_init = glfwInit();
  if (!glfw_init) {
    exit(EXIT_FAILURE);
  }

  //glfw settings
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  use_virtual_sync_ = use_virtual_sync;
  fullscreen_ = fullscreen;
  window_ = glfwCreateWindow(width, height, title,
                             fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
  if (!window_) {
    glfwTerminate();
    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 4.1?");
  }
  glfwGetWindowSize(window_, &width_, &height_);
  glfwGetWindowPos(window_, &x_, &y_);
  glfwSetWindowUserPointer(window_, this);
  glfwMakeContextCurrent(window_);

  // initialise GLEW
  glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
  static bool glew_init = glewInit();
  if (glew_init != GLEW_OK)
    throw std::runtime_error("glewInit failed");

  // make sure OpenGL version 4.1 API is available
  if (!GLEW_VERSION_4_1)
    throw std::runtime_error("OpenGL 4.1 API is not available.");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  camera_position_ = camera_position_pref = glm::vec3(190, 100, 0);
  camera_direction_ = camera_direction_pref = glm::vec3(0, 0, 1);
  resize(width, height);

  //setup event handlers
  glfwSetWindowSizeCallback(window_, &resize_);
}

Window::~Window() { }

void Window::close() {
  glfwDestroyWindow(window_);
}

void Window::update(double dt) {
  handle_input(dt);
  camera(glm::lookAt(camera_position_, camera_position_ + camera_direction_, glm::vec3(0, 1, 0)));
}
void Window::render(double dt, ecs::EntityManager& entities, std::vector<Renderer> &renderers) {
  glClearColor(0, 0, 0, 1); // black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Render renderers
  for (auto &&renderer : renderers) {
    renderer.use();
    entities.with([&](Position& pos, Rotation& rot, Scale& sca, Model& model){
      renderer.render(dt, *this, pos, rot, sca, model);

    });
    renderer.stopUsing();
  }

  glfwSwapBuffers(window_);
}

void Window::resize_(GLFWwindow* window, int width, int height) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))->resize(width, height);
}

void Window::resize(int width, int height) {
  width_ = width;
  height_ = height;
  projection(glm::perspective(glm::radians(50.0f), ((float) width_) / ((float) height_), 10.0f, 10000.0f));
}

void Window::handle_input(float dt) {
  constexpr float DIR_BLEND = 0.6f, POS_BLEND = 0.8f, SPEED = 500;
  double x_pos, y_pos;
  glfwPollEvents();
  glfwGetCursorPos(window_, &x_pos, &y_pos);
  glm::vec3 y_rotation_axis = glm::normalize(glm::cross(camera_direction_ , glm::vec3(0,-1,0)));
  glm::vec3 x_rotation_axis = glm::normalize(glm::cross(y_rotation_axis, camera_direction_));

  if(glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS){
    camera_position_pref += camera_direction_ * SPEED * dt;
  }

  if(glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS){
    camera_position_pref -= camera_direction_ * SPEED * dt;;
  }

  if(glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS){
    camera_position_pref -= y_rotation_axis * SPEED * dt;;
  }

  if(glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS){
    camera_position_pref += y_rotation_axis * SPEED * dt;;
  }

  if(glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS){
    camera_position_pref += glm::vec3(0, 1, 0) * SPEED * dt;;
  }

  if(glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS){
    camera_position_pref -= glm::vec3(0, 1, 0) * SPEED * dt;;
  }
  camera_position_ = camera_position_ * POS_BLEND + (1 - POS_BLEND) * camera_position_pref;



  if(glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_1)){
    glm::vec4 res = glm::rotate(glm::rotate(glm::mat4(), (float)((x_pos - x_pos_last_) * M_PI/1000), x_rotation_axis),
                                                         (float)((y_pos - y_pos_last_) * M_PI/1000), y_rotation_axis)
                                                          * glm::vec4(camera_direction_pref, 0);
    camera_direction_pref = glm::vec3(res.x, res.y, res.z);
    camera_direction_ = camera_direction_ * DIR_BLEND + (1 - DIR_BLEND) * camera_direction_pref;
  }
  camera_direction_ = glm::normalize(camera_direction_);
  if(camera_direction_.y >  0.8f)  camera_direction_.y =  0.8f;
  if(camera_direction_.y < -0.8f)  camera_direction_.y = -0.8f;
  camera_direction_ = glm::normalize(camera_direction_);
  x_pos_last_ = x_pos;
  y_pos_last_ = y_pos;
  if (glfwWindowShouldClose(window_)) {
    closed_ = true;
  }
}