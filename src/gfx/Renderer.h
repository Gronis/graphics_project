#pragma once

#include <GL/glew.h>

#include "Shader.h"
#include "Mesh.h"

namespace engine {
    namespace gfx{
        class Window;
        class Renderer {
        public:
            Renderer(Program& program);
            
            void use();
            void stopUsing();
            void render(float dt, Window& window, Mesh& mesh);
            
        private:
            Program& program_;
        };
    }
}