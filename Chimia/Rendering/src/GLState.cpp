#include "GLState.h"

#include "OpenGLDefs.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

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
  glBindFramebuffer(GL_FRAMEBUFFER, id);
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
  glUseProgram(id);
}

// --------------------------------------------------------------------------------------
