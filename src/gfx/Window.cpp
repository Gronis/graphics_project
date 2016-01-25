#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Engine.h"
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

  camera(glm::lookAt(glm::vec3(190, 100, 0), glm::vec3(0, 150, 0), glm::vec3(0, 1, 0)));
  projection(glm::perspective(glm::radians(50.0f), ((float) width_) / ((float) height_), 50.0f, 10000.0f));
}

Window::~Window() { }

void Window::close() {
  glfwDestroyWindow(window_);
}

void Window::update(double dt) {
  camera_ = glm::rotate(camera_, 0.2f * (float)dt , glm::vec3(0.0f, 0.1f, 0.0f));
  glfwPollEvents();
  if (glfwWindowShouldClose(window_)) {
    closed_ = true;
  }
}
void Window::render(double dt, std::vector<Model> &models, std::vector<Renderer> &renderers) {
  glClearColor(0, 0, 0, 1); // black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Render renderers
  for (auto &&renderer : renderers) {
    renderer.use();
    for (auto &&model : models) {
      renderer.render(dt, *this, model);
    }
    renderer.stopUsing();
  }

  glfwSwapBuffers(window_);
}