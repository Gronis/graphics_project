#include <iostream>
#include <glm/glm.hpp>

#include "thirdparty/ecs.h"
#include "Engine.h"
#include "Resource.h"
#include "Components.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "gfx/Window.h"



using namespace engine;
using namespace engine::gfx;
using namespace engine::util;

using namespace ecs;

void OnError(int errorCode, const char *msg) {
  throw std::runtime_error(msg);
}

int main() {
  EntityManager entities;
  Window window("graphics_project", 1280, 720);
  Resource resource;
  ShaderProgram program({resource.load<Shader>("res/vertex-shader.vert"),
                         resource.load<Shader>("res/fragment-shader.frag")});
  std::vector<Renderer> renderers = { Renderer(program) };

  auto e = entities.create<Renderable>(glm::vec3(3000,0,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/sibenik/sibenik2.obj"));
  e.get<Model>().genVertexArrayObject(program);

  e = entities.create<Renderable>(glm::vec3(0,0,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/dabrovic-sponza/sponza2.obj"));
  e.get<Model>().genVertexArrayObject(program);

  e = entities.create<Renderable>(glm::vec3(-150,80,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/box.obj"));
  e.get<Model>().genVertexArrayObject(program);
  e.add<RotationVelocity>(0,1,0);

  e = entities.create<Renderable>(glm::vec3(0,0,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/mitsuba/mitsuba-sphere.obj"));
  e.get<Model>().genVertexArrayObject(program);
  e.add<RotationVelocity>(0,1,0);

  Engine engine([&](float dt) {
    entities.with([&](RotationVelocity& vel, Rotation& rot){
      rot += vel * dt;
    });
    window.update(dt);
    window.render(dt, entities, renderers);
    return !window.closed();
  });

  e.add<Texture>(resource.load<Bitmap>("res/wooden-crate.jpg"));


  // print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;


  engine.run();
  return 0;

}