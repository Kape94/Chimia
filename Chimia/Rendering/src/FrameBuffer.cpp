#include "FrameBuffer.h"

#include "BufferPrivate.h"
#include "GLState.h"
#include "OpenGLDefs.h"

#include <cassert>

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

FrameBuffer::~FrameBuffer()
{
  Clear();
}

//---------------------------------------------------------------------------------------

std::shared_ptr<FrameBuffer>
FrameBuffer::Create(const unsigned width, const unsigned height)
{
  std::shared_ptr<FrameBuffer> newFrameBuffer(new FrameBuffer);

  glGenFramebuffers(1, &newFrameBuffer->m_id);

  GLState::BindFramebuffer(newFrameBuffer->m_id);

  newFrameBuffer->m_frameTexture =
    Texture2D::Create(nullptr /*data*/, width, height);

  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D,
    BufferPrivate::GetTextureID(newFrameBuffer->m_frameTexture),
    0);

  glGenRenderbuffers(1, &newFrameBuffer->m_renderBufferId);
  GLState::BindRenderbuffer(newFrameBuffer->m_renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER,
                            newFrameBuffer->m_renderBufferId);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    assert(false && "Framebuffer::Create: Error creating new framebuffer");
  }

  GLState::BindFramebuffer(0);

  return newFrameBuffer;
}

//---------------------------------------------------------------------------------------

void
FrameBuffer::Use() const
{
  GLState::BindFramebuffer(m_id);
}

//---------------------------------------------------------------------------------------

const Texture2DInstance&
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

const FrameBufferInstance&
FrameBuffer::DefaultFrameBuffer()
{
  static FrameBufferInstance defaultFramebuffer(new FrameBuffer);
  return defaultFramebuffer;
}

//---------------------------------------------------------------------------------------
