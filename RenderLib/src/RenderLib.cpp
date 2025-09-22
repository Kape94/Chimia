#include "RenderLib.h"

#include "OpenGLDefs.h"

//---------------------------------------------------------------------------------------

void
RenderLib::Initialize()
{
  glewExperimental = true;
  glewInit();
}

//---------------------------------------------------------------------------------------

void
RenderLib::SetViewport(const unsigned x,
                       const unsigned y,
                       const unsigned width,
                       const unsigned height)
{
  glViewport(x, y, width, height);
}

//---------------------------------------------------------------------------------------

void
RenderLib::Clear(const float r, const float g, const float b, const float a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//---------------------------------------------------------------------------------------
