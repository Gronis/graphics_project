#pragma once
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "util/Manager.h"
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
            void render(double dt, std::vector<Model> &model, std::vector<Renderer> &renderers);
            
            inline int x(){return x_; }
            inline int y(){return y_; }
            inline int width(){ return width_; }
            inline int height(){ return height_; }
            inline float ratio(){ return width() / (float) height(); }
            inline bool closed(){ return closed_;}
            inline glm::mat4 camera() { return camera_; }
            inline void camera(glm::mat4 camera) { camera_ = camera; }
            inline glm::mat4 projection() { return projection_; }
            inline void projection(glm::mat4 projection) { projection_ = projection; }
            
        private:
            int y_;
            int x_;
            int width_;
            int height_;
            bool focused_ = false;
            bool closed_ = false;
            
            bool use_virtual_sync_;
            bool fullscreen_;
            
            glm::mat4 camera_;
            glm::mat4 projection_;
            
            GLFWwindow* window_;
        };
    }
}
