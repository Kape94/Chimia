#include "GLState.h"

#include "OpenGLDefs.h"
#include <limits>

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

namespace GLStateCache {

const unsigned INVALID_ID = std::numeric_limits<unsigned>::max();

unsigned g_currentShader = INVALID_ID;
unsigned g_currentFrameBuffer = INVALID_ID;

}

// --------------------------------------------------------------------------------------

void
GLState::BindVertexArray(const unsigned id)
{
  glBindVertexArray(id);
}

// --------------------------------------------------------------------------------------

void
GLState::BindArrayBuffer(const unsigned id)
{
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

// --------------------------------------------------------------------------------------

void
GLState::BindElementBuffer(const unsigned id)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

// --------------------------------------------------------------------------------------

void
GLState::BindFramebuffer(const unsigned id)
{
  if (id != GLStateCache::g_currentFrameBuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    GLStateCache::g_currentFrameBuffer = id;
  }
}

// --------------------------------------------------------------------------------------

void
GLState::BindRenderbuffer(const unsigned id)
{
  glBindRenderbuffer(GL_RENDERBUFFER, id);
}

// --------------------------------------------------------------------------------------

void
GLState::BindTexture2D(const unsigned id)
{
  glBindTexture(GL_TEXTURE_2D, id);
}

// --------------------------------------------------------------------------------------

void
GLState::BindShader(const unsigned id)
{
  if (id != GLStateCache::g_currentShader) {
    glUseProgram(id);
    GLStateCache::g_currentShader = id;
  }
}

// --------------------------------------------------------------------------------------
