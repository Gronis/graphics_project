#include <iostream>
#include <glm/glm.hpp>

#include "Engine.h"
#include "Resource.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "gfx/Window.h"
#include "gfx/Model.h"

#include "thirdparty/tiny_obj_loader.h"

using namespace engine;
using namespace engine::gfx;
using namespace engine::util;

void OnError(int errorCode, const char *msg) {
  throw std::runtime_error(msg);
}

int main() {
  Window window("graphics_project", 1280, 720);
  Resource resource;
  ShaderProgram program({resource.load<Shader>("res/vertex-shader.vert"),
                         resource.load<Shader>("res/fragment-shader.frag")});
  Texture texture(resource.load<Bitmap>("res/wooden-crate.jpg"));
  std::vector<Renderer> renderers = { Renderer(program) };

  Model model = resource.load<Model>("res/crytek-sponza/sponza.obj");
  Model model2 = resource.load<Model>("res/crytek-sponza/banner.obj");
  model.genVertexArrayObject(program);
  model2.genVertexArrayObject(program);
  std::vector<Model> models = { model };

  Engine engine([&window, &models, &renderers](float dt) {
      window.update(dt);
      window.render(dt, models, renderers);
      return !window.closed();
    });


  // print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;


  engine.run();
  return 0;

}