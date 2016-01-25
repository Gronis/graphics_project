//
// Created by Robin Grönberg on 25/01/16.
//

#include <thirdparty/tiny_obj_loader.h>
#include <glm/common.hpp>
#include <glm/detail/func_geometric.hpp>
#include "Model.h"

namespace engine {
namespace gfx {

void Model::calculate_normals() {
  //TODO: Not working right now, regenerate normals with other software or reimplement this
  for (auto &&shape : this->shapes_) {
    if (shape.mesh.normals.size() == 0) {
      assert(shape.mesh.indices.size() % 3 == 0 && "Indices not as a tuple of 3, something is wrong with this model");
      glm::vec3 *face_normals = new glm::vec3[shape.mesh.indices.size() / 3];
      auto &m = shape.mesh;
      m.normals.reserve(m.positions.size());

      //Calculate normal for each face
      for (int i = 0; i < m.indices.size(); i += 3) {
        glm::vec3 v0(m.positions[m.indices[i] * 3],
                     m.positions[m.indices[i] * 3 + 1],
                     m.positions[m.indices[i] * 3 + 2]);
        glm::vec3 v1(m.positions[m.indices[i + 1] * 3],
                     m.positions[m.indices[i + 1] * 3 + 1],
                     m.positions[m.indices[i + 1] * 3 + 2]);
        glm::vec3 v2(m.positions[m.indices[i + 2] * 3],
                     m.positions[m.indices[i + 2] * 3 + 1],
                     m.positions[m.indices[i + 2] * 3 + 2]);
        v1 = v1 - v0;
        v2 = v2 - v0;
        v0 = glm::normalize(glm::cross(v1, v2));
        face_normals[i / 3] = v0;
      }

      //For each vertex, compute vertex normal
      for (int i = 0; i < m.positions.size() / 3; ++i) {
        glm::vec3 normal;
        //find all triangles connected to vertex
        for (int j = 0; j < m.indices.size(); ++j) {
          //found index pointing to vertex
          if (m.indices[j] == i) {
            normal += face_normals[i];
          }
        }
        if (normal.x != 0 || normal.y != 0 || normal.z != 0) {
          normal = glm::normalize(normal);
        }
        m.normals.push_back(normal.x);
        m.normals.push_back(normal.y);
        m.normals.push_back(normal.z);
      }
      delete face_normals;
    }
  }

}

Model::Model(std::vector<tinyobj::shape_t> shapes, std::vector<tinyobj::material_t> materials) :
  shapes_(shapes),
  materials_(materials) {
  calculate_normals();
}

void Model::genVertexArrayObject(ShaderProgram &program) {
  for (int i = 0; i < shapes_.size(); ++i) {
    auto &m = shapes_[i].mesh;
    auto buffer_size = 8 * m.positions.size() / 3;
    //Data format: X, Y, Z, U, V, NX, NY, NZ
    GLfloat *vbd = new GLfloat[buffer_size];
    for (int j = 0; j < buffer_size / 8; ++j) {
      vbd[j * 8    ] = m.positions[j * 3    ];
      vbd[j * 8 + 1] = m.positions[j * 3 + 1];
      vbd[j * 8 + 2] = m.positions[j * 3 + 2];
      if(m.texcoords.size() > 0){
        vbd[j * 8 + 3] = m.texcoords[j * 2    ];
        vbd[j * 8 + 4] = m.texcoords[j * 2 + 1];
      }
      vbd[j * 8 + 5] = m.normals  [j * 3    ];
      vbd[j * 8 + 6] = m.normals  [j * 3 + 1];
      vbd[j * 8 + 7] = m.normals  [j * 3 + 2];
    }
    // make and bind the VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    vaos_.push_back(vao);

    // make and bind the VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffer_size, vbd, GL_STATIC_DRAW);
    vbo_vertex_data_.push_back(vbo);
    delete vbd;

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(program.attrib("vert"));
    glVertexAttribPointer(program.attrib("vert"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
    // Bind our third VBO as being the active buffer and storing vertex array indicies

    glEnableVertexAttribArray(program.attrib("vertTexCoord"));
    glVertexAttribPointer(program.attrib("vertTexCoord"),
                          2,
                          GL_FLOAT,
                          GL_TRUE,
                          8 * sizeof(GLfloat),
                          (const GLvoid *) (3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(program.attrib("vertNormal"));
    glVertexAttribPointer(program.attrib("vertNormal"),
                          3,
                          GL_FLOAT,
                          GL_TRUE,
                          8 * sizeof(GLfloat),
                          (const GLvoid *) (5 * sizeof(GLfloat)));

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(GLuint), m.indices.data(), GL_STATIC_DRAW);
    vbo_indicies_.push_back(vbo);

    // unbind the VBOs and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

  }
}

} // gfx
} // engine


