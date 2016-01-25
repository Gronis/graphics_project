#include <thirdparty/tiny_obj_loader.h>
#include "Renderer.h"
#include "Window.h"
#include "Model.h"

using namespace engine::gfx;

Renderer::Renderer(ShaderProgram & program) : program_(program){
    
}

void Renderer::use(){
    program_.use();
}

void Renderer::stopUsing(){
    program_.stopUsing();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::render(float dt, Window& window, Model& model){
    program_.setUniform("camera", window.camera());
    program_.setUniform("projection", window.projection());
    
    //Texture
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, mesh.texture().ref());
    //program_.setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    for (int i = 0; i < model.getNumOfShapes(); ++i) {
        glBindVertexArray(model.getVao(i));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getVboIndicies(i));
        //glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount(i));
        glDrawElements(GL_TRIANGLES, model.getIndiciesCount(i), GL_UNSIGNED_INT, NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

}

