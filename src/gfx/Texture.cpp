#include "Texture.h"
#include <stdexcept>
#include <bitset>

using namespace engine::gfx;

GLuint Texture::active_texture_unit_ = 0;

static GLenum TextureFormatForBitmapFormat(Bitmap::Format format)
{
    switch (format) {
        case Bitmap::Format_Grayscale: return GL_LUMINANCE;
        case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
        case Bitmap::Format_RGB: return GL_RGB;
        case Bitmap::Format_RGBA: return GL_RGBA;
        default: throw std::runtime_error("Unrecognised Bitmap::Format");
    }
}

Texture::Texture(const Bitmap& bitmap, GLint minMagFiler, GLint wrapMode) :
  width_((GLfloat)bitmap.width()),
  height_((GLfloat)bitmap.height()) {
    glGenTextures(1, &ref_);
    glBindTexture(GL_TEXTURE_2D, ref_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 TextureFormatForBitmapFormat(bitmap.format()),
                 (GLsizei)bitmap.width(),
                 (GLsizei)bitmap.height(),
                 0,
                 TextureFormatForBitmapFormat(bitmap.format()),
                 GL_UNSIGNED_BYTE,
                 bitmap.pixelBuffer());
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &ref_);
}

GLuint Texture::bind() {
  GLuint used_texture_unit = active_texture_unit_;
  GLint max_tex_units;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_tex_units);
  glActiveTexture(GL_TEXTURE0 + active_texture_unit_++);
  glBindTexture(GL_TEXTURE_2D, ref());
  active_texture_unit_ %= max_tex_units;
  return used_texture_unit;
}

GLuint Texture::ref() const
{
    return ref_;
}

GLfloat Texture::width() const
{
    return width_;
}

GLfloat Texture::height() const
{
    return height_;
}
