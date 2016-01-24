#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Engine.h"
#include "Mesh.h"
#include "gfx/Texture.h"

using namespace engine::gfx;

Window::Window(const char* title, int width, int height,bool use_virtual_sync, bool fullscreen){
    //init glfw
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }
    
    //glfw settings
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    _use_virtual_sync = use_virtual_sync;
    _fullscreen = fullscreen;
    window_ = glfwCreateWindow(width, height, title,
                               fullscreen? glfwGetPrimaryMonitor() : NULL, NULL);
    if (!window_){
        glfwTerminate();
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    }
    glfwGetWindowSize(window_, &width_, &height_);
    glfwGetWindowPos(window_, &_x, &_y);
    glfwSetWindowUserPointer(window_, this);
    glfwMakeContextCurrent(window_);
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //camera(glm::mat4(1.0));
    //projection(glm::mat4(1.0));
    camera(glm::lookAt(glm::vec3(3,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0)));
    projection(glm::perspective(glm::radians(50.0f), ((float)width_)/((float)height_), 0.1f, 10.0f));
    
}

Window::~Window(){}

void Window::close(){
    glfwDestroyWindow(window_);
}

void Window::update(double dt){
    glfwPollEvents();
    if(glfwWindowShouldClose(window_)){
        _closed = true;
    }
}
void Window::render(double dt, util::Manager<Mesh> &meshes, util::Manager<Renderer> &renderers){
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Render renderers
    for(auto renderer : renderers){
        renderer->use();
        for( auto mesh : meshes) {
            renderer->render(dt, *this, mesh.get());
        }
        renderer->stopUsing();
    }
    
    glfwSwapBuffers(window_);
}