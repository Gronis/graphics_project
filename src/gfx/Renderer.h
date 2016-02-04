#pragma once

#include <GL/glew.h>

#include "Shader.h"
#include "Model.h"

namespace engine {
    namespace gfx{
        class Window;
        class Renderer {
        public:
            Renderer(ShaderProgram & program);
            
            void use();
            void stopUsing();
            void render(float dt, Window& window,glm::vec3& pos, glm::vec3& rot, glm::vec3& sca, Model& model);
            
        private:
            ShaderProgram & program_;
        };
    }
}