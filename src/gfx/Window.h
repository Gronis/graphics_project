#pragma once
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "util/Manager.h"
#include "Mesh.h"
#include "Renderer.h"

namespace engine{
    namespace gfx{
        class Window{
            
        public:
            Window(const char* title, int width, int height, bool use_virtual_sync, bool fullscreen);
            Window(const char* title, int width, int height) : Window(title, width, height, true , false){}
            ~Window();
            
            void close();
            void update(double dt);
            void render(double dt, util::Manager<Mesh> &meshes, util::Manager<Renderer> &renderers);
            
            inline int x(){return _x; }
            inline int y(){return _y; }
            inline int width(){ return width_; }
            inline int height(){ return height_; }
            inline float ratio(){ return width() / (float) height(); }
            inline bool closed(){ return _closed;}
            inline glm::mat4 camera() { return camera_; }
            inline void camera(glm::mat4 camera) { camera_ = camera; }
            inline glm::mat4 projection() { return projection_; }
            inline void projection(glm::mat4 projection) { projection_ = projection; }
            
        private:
            int _y;
            int _x;
            int width_;
            int height_;
            bool _focused;
            bool _closed = false;
            
            bool _use_virtual_sync;
            bool _fullscreen;
            
            glm::mat4 camera_;
            glm::mat4 projection_;
            
            GLFWwindow* window_;
        };
    }
}
