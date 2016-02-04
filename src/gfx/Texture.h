#pragma once

#include <GL/glew.h>

#include "Bitmap.h"

namespace engine {
namespace gfx {
class Texture {
 public:
  Texture(const Bitmap &bitmap,
          GLint minMagFiler = GL_LINEAR,
          GLint wrapMode = GL_WRAP_BORDER);

  Texture();

  //enable move
  Texture(Texture&&);
  Texture& operator=(Texture&&);

  ~Texture();

  GLuint bind();
  GLuint ref() const;
  GLfloat width() const;
  GLfloat height() const;

 private:
  GLuint ref_ = 0;
  GLfloat width_;
  GLfloat height_;
  static GLuint active_texture_unit_;

  //copying disabled
  Texture(const Texture &) = delete;
  const Texture &operator=(const Texture &) = delete;
};
}
}