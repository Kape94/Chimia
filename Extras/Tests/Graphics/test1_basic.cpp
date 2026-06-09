#include "Draw3D/Draw3D.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/Window.h"

#include "TestsUtils.h"

namespace Scenarios {

void
Basic(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  Chimia::Draw3D::Triangle(
    Chimia::Draw3D::VertexPC{ { 0.0f, 0.0f, 0.0f },
                              { 1.0f, 0.0f, 0.0f, 1.0f } },
    Chimia::Draw3D::VertexPC{ { 1.0f, 0.0f, 0.0f },
                              { 0.0f, 1.0f, 0.0f, 1.0f } },
    Chimia::Draw3D::VertexPC{ { 0.0f, 1.0f, 0.0f },
                              { 0.0f, 0.0f, 1.0f, 1.0f } });

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage("test1_basic.png");
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #1");

  TestsUtils::InitTesting(argv, win);

  Chimia::Draw3D::Initialize();

  Scenarios::Basic(win);

  return 0;
}
