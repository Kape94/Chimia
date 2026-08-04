#include "FrameBuffer.h"

#include "BufferPrivate.h"
#include "GLState.h"
#include "OpenGLDefs.h"

#include <cassert>

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

  glGenFramebuffers(1, &m_id);

  GLState::BindFramebuffer(m_id);

  m_frameTexture.Create(nullptr /*data*/, width, height);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         BufferPrivate::GetTextureID(m_frameTexture),
                         0);

  glGenRenderbuffers(1, &m_renderBufferId);
  GLState::BindRenderbuffer(m_renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER,
                            m_renderBufferId);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    assert(false && "Framebuffer::Create: Error creating new framebuffer");
  }

  GLState::BindFramebuffer(0);
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::Use()
{
  GLState::BindFramebuffer(m_id);
}

//---------------------------------------------------------------------------------------

const Texture2D&
FrameBuffer::GetTexture() const
{
  return m_frameTexture;
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::Clear()
{
  if (m_id != 0) {
    glDeleteFramebuffers(1, &m_id);
    m_id = 0;
  }
  if (m_renderBufferId != 0) {
    glDeleteRenderbuffers(1, &m_renderBufferId);
    m_renderBufferId = 0;
  }
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::UseDefaultFrameBuffer()
{
  GLState::BindFramebuffer(0);
}

//---------------------------------------------------------------------------------------
