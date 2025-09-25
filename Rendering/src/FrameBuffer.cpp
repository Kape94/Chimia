#include "FrameBuffer.h"

#include "OpenGLDefs.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

FrameBuffer::FrameBuffer(const unsigned width, const unsigned height)
{
  Create(width, height);
}

//---------------------------------------------------------------------------------------

FrameBuffer::~FrameBuffer()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::Create(const unsigned width, const unsigned height)
{
  Clear();

  glGenFramebuffers(1, &id);

  glBindFramebuffer(GL_FRAMEBUFFER, id);

  frameTexture.Create(nullptr /*data*/, width, height);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         frameTexture.GetId(),
                         0);

  glGenRenderbuffers(1, &renderBufferId);
  glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER,
                            renderBufferId);

  // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  // return 1;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::Use()
{
  glBindFramebuffer(GL_FRAMEBUFFER, id);
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::UseTexture(const TextureUnit& textureUnit)
{
  frameTexture.Use(textureUnit);
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::Clear()
{
  frameTexture.Clear();
  if (id != 0) {
    glDeleteFramebuffers(1, &id);
    id = 0;
  }
  if (renderBufferId != 0) {
    glDeleteRenderbuffers(1, &renderBufferId);
    renderBufferId = 0;
  }
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::UseDefaultFrameBuffer()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//---------------------------------------------------------------------------------------
