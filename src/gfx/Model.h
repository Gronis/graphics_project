//
// Created by Robin Grönberg on 25/01/16.
//

#ifndef GRAPHICS_PROJECT_MODEL_H
#define GRAPHICS_PROJECT_MODEL_H

#include <GL/glew.h>
#include <thirdparty/tiny_obj_loader.h>
#include "thirdparty/tiny_obj_loader.h"
#include "Shader.h"

namespace engine{
namespace gfx{

class Model {
 public:
  Model(std::vector<tinyobj::shape_t> shapes, std::vector<tinyobj::material_t> materials);
  void genVertexArrayObject(ShaderProgram & program);

  const tinyobj::shape_t &getShape(int index) const {
    return shapes_[index];
  }
  const GLuint &getVao(int index) const {
    return vaos_[index];
  }
  int getNumOfShapes() const {
    return shapes_.size();
  }

  int getVertexCount(int index) const{
    return shapes_[index].mesh.positions.size();
  }

  int getIndiciesCount(int index) const{
    return shapes_[index].mesh.indices.size();
  }

  GLuint getVboVertexData(int index) const{
    return vbo_vertex_data_[index];
  }

  GLuint getVboIndicies(int index) const{
    return vbo_indicies_[index];
  }

 private:
  void calculate_normals();

  std::vector<tinyobj::shape_t> shapes_;
  std::vector<tinyobj::material_t> materials_;
  std::vector<GLuint> vaos_;
  std::vector<GLuint> vbo_vertex_data_;
  std::vector<GLuint> vbo_indicies_;
};

} // gfx
} // engine


#endif //GRAPHICS_PROJECT_MODEL_H
