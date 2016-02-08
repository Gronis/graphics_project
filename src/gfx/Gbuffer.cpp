//
// Created by Robin Grönberg on 06/02/16.
//

#include <GL/glew.h>
#include <stdio.h>
#include "Gbuffer.h"

using namespace engine::gfx;

bool Gbuffer::init(unsigned int WindowWidth, unsigned int WindowHeight)
{
  //Delete if initialized before
  if(init_){

    glDeleteFramebuffers(1, &m_fbo);

    glDeleteTextures(GBUFFER_NUM_TEXTURES, &m_fbo);
    glDeleteTextures(1, &m_depthTexture);

  }
  // Create the FBO
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

  // Create the gbuffer textures
  glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);
  glGenTextures(1, &m_depthTexture);

  // Small channel size increases performance.
  constexpr GLuint CHANNELS[] = { GL_R3_G3_B2, GL_R3_G3_B2, GL_R3_G3_B2 };

  // G-buffer channels, attached as color attachment
  for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES ; i++) {
    glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, CHANNELS[i], WindowWidth /*/ 2*/, WindowHeight /*/ 2*/, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
  }

  // depth
  /*
  glGenRenderbuffers(1, &m_dbo);
  glBindRenderbuffer(GL_RENDERBUFFER, m_dbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
  // Associate our created depth buffer with the FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, m_dbo);
  */

  glBindTexture(GL_TEXTURE_2D, m_depthTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, WindowWidth /*/ 2*/, WindowHeight /*/ 2*/, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
               NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

  GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
                           GL_COLOR_ATTACHMENT1,
                           GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);

  GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (Status != GL_FRAMEBUFFER_COMPLETE) {
    printf("FB error, status: 0x%x\n", Status);
    return false;
  }

  // restore default FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  init_ = true;
  return true;
}

Gbuffer::Gbuffer() {

}

Gbuffer::~Gbuffer() {

}

void Gbuffer::bind_read() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    //glBindTexture(GL_TEXTURE_RECTANGLE_ARB, m_textures[i]);
  }
  glActiveTexture(GL_TEXTURE0 + GBUFFER_NUM_TEXTURES);
  glBindTexture(GL_TEXTURE_2D, m_depthTexture);
}

void Gbuffer::bind_write() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void Gbuffer::set_read_buffer(GBUFFER_TEXTURE_TYPE TextureType) {
  glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}