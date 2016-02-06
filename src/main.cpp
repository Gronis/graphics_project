#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "thirdparty/ecs.h"
#include "Engine.h"
#include "Resource.h"
#include "Components.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "gfx/Window.h"
#include "gfx/Gbuffer.h"



using namespace engine;
using namespace engine::gfx;
using namespace engine::util;

using namespace ecs;

void OnError(int errorCode, const char *msg) {
  throw std::runtime_error(msg);
}

GLuint frameBuffer;
GLuint depthBuffer;


void render_model(Window &window, ShaderProgram& program, glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &sca, Model &model) {
//  program_.setUniform("camera", window.camera());
//  program_.setUniform("projection", window.projection());

  auto modelMat = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(), pos),
                                                                 rot.x, glm::vec3(1, 0, 0)),
                                                     rot.y, glm::vec3(0, 1, 0)),
                                         rot.z, glm::vec3(0, 0, 1)), sca);
  auto viewMat = window.camera();
  auto projMat = window.projection();

  glm::mat4 modelViewProj = projMat * viewMat * modelMat;

  program.setUniform("modelViewProj", modelViewProj);
  program.setUniform("modelViewNormal", glm::transpose(glm::inverse(viewMat * modelMat)));

  for (int i = 0; i < model.getNumOfShapes(); ++i) {
    //Texture
    glActiveTexture(GL_TEXTURE0);
    auto diff_tex = model.getMaterial(i).diffuse_tex.get();
    if (diff_tex != nullptr) {
      glBindTexture(GL_TEXTURE_2D, diff_tex->ref());
      program.setUniform("diffuse_tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    }

    glBindVertexArray(model.getVao(i));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getVboIndicies(i));
    //glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount(i));
    glDrawElements(GL_TRIANGLES, model.getIndiciesCount(i), GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

}

GLuint createAddAttribBuffer(GLuint vertexArrayObject, const void *data, const size_t dataSize, GLuint attributeIndex, GLsizei attributeSize, GLenum type, GLenum bufferUsage = GL_STATIC_DRAW)
{
  GLuint buffer = 0;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, dataSize, data, bufferUsage);

  // Now attach buffer to vertex array object.
  glBindVertexArray(vertexArrayObject);
  glVertexAttribPointer(attributeIndex, attributeSize, type, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(attributeIndex);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return buffer;
}


void drawFullScreenQuad()
{
  static GLuint vertexArrayObject = 0;
  static int nofVertices = 4;

  // do this initialization first time the function is called... somewhat dodgy, but works for demonstration purposes
  if (vertexArrayObject == 0)
  {
    glGenVertexArrays(1, &vertexArrayObject);
    static const float positions[] = {
        -1, -1,
        1, -1,
        -1,  1,
        1,  1,
    };
    createAddAttribBuffer(vertexArrayObject, positions, sizeof(positions), 0, 2, GL_FLOAT);
  }

  glBindVertexArray(vertexArrayObject);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nofVertices);
}


int main() {
  EntityManager entities;
  Window window("graphics_project", 960, 540);
  Resource resource;
  ShaderProgram geometry_shader_program({resource.load<Shader>("res/vertex-shader.vert"),
                                         resource.load<Shader>("res/fragment-shader.frag")});

  ShaderProgram deferred_shader_program({resource.load<Shader>("res/deferred-shading.vert"),
                                         resource.load<Shader>("res/deferred-shading.frag")});

  auto e = entities.create<Renderable>(glm::vec3(3000,0,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/sibenik/sibenik2.obj"));
  e.get<Model>().genVertexArrayObject(geometry_shader_program);

  e = entities.create<Renderable>(glm::vec3(0,0,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/dabrovic-sponza/sponza2.obj"));
  e.get<Model>().genVertexArrayObject(geometry_shader_program);

  e = entities.create<Renderable>(glm::vec3(-150,80,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/box.obj"));
  e.get<Model>().genVertexArrayObject(geometry_shader_program);
  e.add<RotationVelocity>(0,1,0);

  e = entities.create<Renderable>(glm::vec3(0,0,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(50,50,50),
                                  resource.load<Model>("res/mitsuba/mitsuba-sphere.obj"));
  e.get<Model>().genVertexArrayObject(geometry_shader_program);
  e.add<RotationVelocity>(0,1,0);

  Engine engine([&](float dt) {
    entities.with([&](RotationVelocity& vel, Rotation& rot){
      rot += vel * dt;
    });
    window.update(dt);

    //Render
    window.gbuffer().bind_write();
    geometry_shader_program.use();

    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    entities.with([&](Position& pos, Rotation& rot, Scale& sca, Model& model){
      render_model(window, geometry_shader_program, pos, rot, sca, model);
    });
    geometry_shader_program.stopUsing();


    //Post Processing
    deferred_shader_program.use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window.gbuffer().bind_read();
    deferred_shader_program.setUniform("frameBufferBaseColor", 0);
    deferred_shader_program.setUniform("frameBufferPBS", 1);
    deferred_shader_program.setUniform("frameBufferNormal", 2);
    deferred_shader_program.setUniform("frameBufferDepth", 3);
    drawFullScreenQuad();
    deferred_shader_program.stopUsing();

    window.swap_buffers();

    engine.sleeping(!window.focused());
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