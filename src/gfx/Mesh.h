#pragma once

#include "Shader.h"
#include "Texture.h"

namespace engine {
    namespace gfx{
        class Mesh{
        public:
            Mesh(GLfloat* vertexData, int vertexCount, Texture& texture, Program& program):
            vertex_count_(vertexCount), texture_(texture){
                // make and bind the VAO
                glGenVertexArrays(1, &vao_);
                glBindVertexArray(vao_);
                
                // make and bind the VBO
                glGenBuffers(1, &vbo_);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_count_ * 5, vertexData, GL_STATIC_DRAW);
                
                // connect the xyz to the "vert" attribute of the vertex shader
                glEnableVertexAttribArray(program.attrib("vert"));
                glVertexAttribPointer(program.attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
                
                glEnableVertexAttribArray(program.attrib("vertTexCoord"));
                glVertexAttribPointer(program.attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
                
                // unbind the VBO and VAO
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
                
                /* Do in renderer
                 
                 */
            }
            inline GLuint vao(){ return vao_; }
            inline GLuint vbo(){ return vbo_; }
            inline int vertex_count(){ return vertex_count_; }
            inline Texture& texture(){ return texture_; }
        private:
            GLuint vao_;
            GLuint vbo_;
            int vertex_count_;
            Texture& texture_;
        };
    }
}