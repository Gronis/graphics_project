#pragma once

#include <GL/glew.h>

#include "Bitmap.h"

namespace engine {
    namespace gfx{
        class Texture {
        public:
            Texture(const Bitmap& bitmap,
                    GLint minMagFiler = GL_LINEAR,
                    GLint wrapMode = GL_CLAMP_TO_EDGE);
            
            ~Texture();
            
            GLuint ref() const;
            GLfloat width() const;
            GLfloat height() const;
            
        private:
            GLuint ref_;
            GLfloat width_;
            GLfloat height_;
            
            //copying disabled
            Texture(const Texture&) = delete;
            const Texture& operator=(const Texture&) = delete;
        };
    }
}