#include <thirdparty/tiny_obj_loader.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"
#include "Window.h"
#include "Model.h"

using namespace engine::gfx;

Renderer::Renderer(ShaderProgram &program) : program_(program) {

}

void Renderer::use() {
  program_.use();
}

void Renderer::stopUsing() {
  program_.stopUsing();
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::render(float dt, Window &window, glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &sca, Model &model) {
//  program_.setUniform("camera", window.camera());
//  program_.setUniform("projection", window.projection());

  auto modelMat = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(), pos),
                                                                 rot.x, glm::vec3(1,0,0)),
                                                                 rot.y, glm::vec3(0,1,0)),
                                                                 rot.z, glm::vec3(0,0,1)),sca);
  auto viewMat = window.camera();
  auto projMat = window.projection();

  glm::mat4 modelViewProj = projMat * viewMat * modelMat;

  program_.setUniform("modelViewProj", modelViewProj);
  program_.setUniform("modelViewNormal", glm::transpose(glm::inverse(viewMat * modelMat)));

  for (int i = 0; i < model.getNumOfShapes(); ++i) {
    //Texture
    glActiveTexture(GL_TEXTURE0);
    auto diff_tex = model.getMaterial(i).diffuse_tex.get();
    if(diff_tex != nullptr){
      glBindTexture(GL_TEXTURE_2D, diff_tex->ref());
      program_.setUniform("diffuse_tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    }

    glBindVertexArray(model.getVao(i));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getVboIndicies(i));
    //glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount(i));
    glDrawElements(GL_TRIANGLES, model.getIndiciesCount(i), GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

}

