#include "Renderer.h"
#include "Window.h"

using namespace engine::gfx;

Renderer::Renderer(Program& program) : program_(program){
    
}

void Renderer::use(){
    program_.use();
}

void Renderer::stopUsing(){
    program_.stopUsing();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::render(float dt, Window& window, Mesh& mesh){
    program_.setUniform("camera", window.camera());
    program_.setUniform("projection", window.projection());
    
    //Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.texture().ref());
    program_.setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    
    glBindVertexArray(mesh.vao());
    
    /*
    glEnableVertexAttribArray(program_.attrib("vert"));
    glVertexAttribPointer(program_.attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
    
    glEnableVertexAttribArray(program_.attrib("vertTexCoord"));
    glVertexAttribPointer(program_.attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
    */
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count());
    
    glBindVertexArray(0);
}

