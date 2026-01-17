#include "Rendering.h"

#include "OpenGLDefs.h"

//---------------------------------------------------------------------------------------

void
Chimia::Rendering::Initialize()
{
  glewExperimental = true;
  glewInit();
}

//---------------------------------------------------------------------------------------

void
Chimia::Rendering::SetViewport(const unsigned x,
                               const unsigned y,
                               const unsigned width,
                               const unsigned height)
{
  glViewport(x, y, width, height);
}

//---------------------------------------------------------------------------------------

void
Chimia::Rendering::Clear(const float r,
                         const float g,
                         const float b,
                         const float a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//---------------------------------------------------------------------------------------

// TODO: Rendering samples seem to not be using correct winding. Test enabling
// depth test on them
void
Chimia::Rendering::EnableDepthTest(const bool enable)
{
  constexpr GLenum glDepthTestOption = GL_DEPTH_TEST;

  if (enable) {
    glEnable(glDepthTestOption);
  } else {
    glDisable(glDepthTestOption);
  }
}

//---------------------------------------------------------------------------------------