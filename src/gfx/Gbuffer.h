//
// Created by Robin Grönberg on 06/02/16.
//

#ifndef GRAPHICS_PROJECT_GBUFFER_H
#define GRAPHICS_PROJECT_GBUFFER_H

namespace engine{

namespace gfx{

class Gbuffer {
 public:
  enum GBUFFER_TEXTURE_TYPE {
    GBUFFER_TEXTURE_TYPE_BASE_COLOR,
    GBUFFER_TEXTURE_TYPE_PBS, //metallic, specular roughness
    GBUFFER_TEXTURE_TYPE_NORMAL,
    GBUFFER_NUM_TEXTURES
  };

  Gbuffer();

  ~Gbuffer();

  bool init(unsigned int WindowWidth, unsigned int WindowHeight);

  void bind_write();

  void bind_read();

  void set_read_buffer(GBUFFER_TEXTURE_TYPE TextureType);

 private:

  GLuint m_fbo;
  GLuint m_dbo;
  GLuint m_textures[GBUFFER_NUM_TEXTURES];
  GLuint m_depthTexture;
  bool init_ = false;
};


} //namespace gfx

} //namespace engine

#endif //GRAPHICS_PROJECT_GBUFFER_H
