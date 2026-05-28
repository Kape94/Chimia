#include "App/App.h"

//-------------------------------------------------------------------

class SimpleTriangleDrawer
  :
  /*implements*/ public Chimia::App::IAppRenderer
{
public:
  void Render() override
  {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
  }
};

//-------------------------------------------------------------------

int
main()
{
  Chimia::App::IApp& app = Chimia::App::GetApp();

  SimpleTriangleDrawer drawer;
  app.SetImpl(&drawer);

  app.Run();

  return 0;
}

//-------------------------------------------------------------------
