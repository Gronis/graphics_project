//
// Created by Robin Grönberg on 25/01/16.
//

#ifndef GRAPHICS_PROJECT_MODEL_H
#define GRAPHICS_PROJECT_MODEL_H

#include <GL/glew.h>
#include <thirdparty/tiny_obj_loader.h>
#include "thirdparty/tiny_obj_loader.h"
#include "Shader.h"
#include "Bitmap.h"
#include "Texture.h"

namespace engine{
namespace gfx{

class Model {
 public:
  class Material{
   public:
    glm::vec3 ambient_;            // map_Ka
    glm::vec3 diffuse_;            // map_Kd
    glm::vec3 specular_;           // map_Ks
    glm::vec3 specular_highlight_; // map_Ns
    glm::vec3 bump_;               // map_bump, bump
    glm::vec3 displacement_;       // disp
    glm::vec3 alpha_;              // map_d

    std::shared_ptr<Texture> ambient_tex;            // map_Ka
    std::shared_ptr<Texture> diffuse_tex;            // map_Kd
    std::shared_ptr<Texture> specular_tex;           // map_Ks
    std::shared_ptr<Texture> specular_highlight_tex; // map_Ns
    std::shared_ptr<Texture> bump_tex;               // map_bump, bump
    std::shared_ptr<Texture> displacement_tex;       // disp
    std::shared_ptr<Texture> alpha_tex;              // map_d
  };

  Model(std::vector<tinyobj::shape_t>     shapes,
        std::vector<tinyobj::material_t>  materials,
        std::vector<Material>             materials_ref);

  void genVertexArrayObject(ShaderProgram & program);

  inline const tinyobj::shape_t &getShape(int index) const {
    return shapes_[index];
  }
  inline const GLuint &getVao(int index) const {
    return vaos_[index];
  }
  inline int getNumOfShapes() const {
    return shapes_.size();
  }

  inline int getVertexCount(int index) const{
    return shapes_[index].mesh.positions.size();
  }

  inline int getIndiciesCount(int index) const{
    return shapes_[index].mesh.indices.size();
  }

  inline GLuint getVboVertexData(int index) const{
    return vbo_vertex_data_[index];
  }

  inline GLuint getVboIndicies(int index) const{
    return vbo_indicies_[index];
  }

  inline const Material& getMaterial(int index) const{
    return materials_ref_[shapes_[index].mesh.material_ids[0]];
  }

 private:
  void calculate_normals();

  std::vector<tinyobj::shape_t> shapes_;
  std::vector<tinyobj::material_t> materials_;
  std::vector<GLuint> vaos_;
  std::vector<GLuint> vbo_vertex_data_;
  std::vector<GLuint> vbo_indicies_;
  std::vector<Material> materials_ref_;
};

} // gfx
} // engine


#endif //GRAPHICS_PROJECT_MODEL_H
