#include <iostream>
#include <glm/glm.hpp>

#include "Engine.h"
#include "Resource.h"
#include "gfx/Shader.h"
#include "gfx/Mesh.h"
#include "gfx/Texture.h"

using namespace engine;
using namespace engine::gfx;
using namespace engine::util;

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

GLfloat vertexData[] = {
    //  X     Y     Z       U     V
    // bottom
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
    
    // top
    -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    
    // front
    -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
    1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    
    // back
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
    1.0f, 1.0f,-1.0f,   1.0f, 1.0f,
    
    // left
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    
    // right
    1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
    1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 1.0f
};

int main(){
    Engine engine;
    Resource resource;

    engine.create_manager<Program>(32);
    engine.create_manager<Mesh>(1024);
    engine.create_manager<Texture>(32);
    engine.create_manager<Window>(1);
    
    engine.create<Window>("voxel-engine", 800, 480);
    
    vector<Shader> shaders = {
        Shader(resource.load<string>("res/vertex-shader.vert"), GL_VERTEX_SHADER),
        Shader(resource.load<string>("res/fragment-shader.frag"), GL_FRAGMENT_SHADER)
    };
    auto program = engine.create<Program>(shaders);
    engine.create<Renderer>(program.get());


    auto texture = engine.create<Texture>(resource.load<Bitmap>("res/wooden-crate.jpg"));
    engine.create<Mesh>(vertexData, 36, texture.get(), program.get());
    
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    engine.run();
    return 0;

}