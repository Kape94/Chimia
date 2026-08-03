#include "FrameBuffer.h"

#include "GLState.h"
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

  GLState::BindFramebuffer(id);

  frameTexture.Create(nullptr /*data*/, width, height);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         frameTexture.GetId(),
                         0);

  glGenRenderbuffers(1, &renderBufferId);
  GLState::BindRenderbuffer(renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER,
                            renderBufferId);

  // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  // return 1;

  GLState::BindFramebuffer(0);
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::Use()
{
  GLState::BindFramebuffer(id);
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
  GLState::BindFramebuffer(0);
}

//---------------------------------------------------------------------------------------
