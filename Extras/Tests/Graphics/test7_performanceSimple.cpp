#include "Draw3D/Draw3D.h"

#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

#include "TestsUtils.h"

void
DrawTriangles(const size_t number)
{
  using namespace ExtrasUtils;

  auto randomVertex = []() {
    return Chimia::Draw3D::VertexPC{
      { Rand(), Rand(), Rand() },
      { Rand(), Rand(), Rand(), Rand() },
    };
  };

  for (size_t i = 0; i < number; ++i) {
    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::Triangle(randomVertex(), randomVertex(), randomVertex());
    Chimia::Draw3D::Flush();
  }
}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #7");

  TestsUtils::InitTesting(argv, win);
  ExtrasUtils::InitRandom();

  Chimia::Draw3D::Initialize();

  constexpr size_t nFrames = 10;
  constexpr size_t nTriangles = 10000;

  ExtrasUtils::Tic();
  for (size_t i = 0; i < nFrames; ++i) {
    DrawTriangles(nTriangles);
  }
  const float execTime = ExtrasUtils::Toc();

  constexpr float MEAN_EXECUTION_TIME = 10.415;
  constexpr float TOLERANCE = 0.005f * MEAN_EXECUTION_TIME;
  TestsUtils::ExpectValue(execTime, MEAN_EXECUTION_TIME, TOLERANCE);

  return 0;
}
