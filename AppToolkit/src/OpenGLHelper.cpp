#include "OpenGLHelper.h"

#include <GL/glew.h>

//-----------------------------------------------------------------------------

_USING_APP_TOOLKIT_NAMESPACE

//-----------------------------------------------------------------------------

void
OpenGLHelper::Init(const unsigned screenWidth, const unsigned screenHeight)
{
  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, screenWidth, screenHeight);
}

//-----------------------------------------------------------------------------

void
OpenGLHelper::Clear(const float r, const float g, const float b)
{
  glClearColor(r, g, b, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------

void
OpenGLHelper::Flush()
{
  glFlush();
}

//-----------------------------------------------------------------------------
