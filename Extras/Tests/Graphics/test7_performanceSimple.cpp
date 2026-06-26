#include "Draw3D/Draw3D.h"

#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

#include "TestsUtils.h"

// ----------------------------------------------------------------------------

void
DrawTriangles(const size_t nTriangles, const size_t nFrames)
{
  using namespace ExtrasUtils;

  auto randomVertex = []() {
    return Chimia::Draw3D::VertexPC{
      { Rand(), Rand(), Rand() },
      { Rand(), Rand(), Rand(), Rand() },
    };
  };

  for (size_t k = 0; k < nFrames; ++k) {
    Chimia::Draw3D::ClearScreen();
    for (size_t i = 0; i < nTriangles; ++i) {
      Chimia::Draw3D::Triangle(randomVertex(), randomVertex(), randomVertex());
    }
    Chimia::Draw3D::Flush();
  }
}

void
AddAndRemoveTriangles(const size_t nTriangles, const size_t nFrames)
{
  using namespace ExtrasUtils;

  std::vector<Chimia::Draw3D::TriangleMeshID> trianglesInstances;

  auto randomVertex = []() {
    return Chimia::Draw3D::VertexPC{
      { Rand(), Rand(), Rand() },
      { Rand(), Rand(), Rand(), Rand() },
    };
  };

  Chimia::Draw3D::ClearScreen();
  for (size_t i = 0; i < nTriangles; ++i) {
    const auto triangleInstance = Chimia::Draw3D::AddRetainedTriangles(
      { randomVertex(), randomVertex(), randomVertex() });
    trianglesInstances.push_back(triangleInstance);
  }
  Chimia::Draw3D::Flush();

  for (size_t i = 0; i < nFrames; ++i) {
    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::Flush();
  }

  for (const auto& id : trianglesInstances) {
    Chimia::Draw3D::DeleteRetainedTriangles(id);
  }

  Chimia::Draw3D::ClearScreen();
  Chimia::Draw3D::Flush();
}

float
PercentualDifference(const float executionTime, const float referenceTime)
{
  const float relativeRate = executionTime / referenceTime;
  return std::abs(1.0f - relativeRate);
}

void
ExpectExecutionTime(const float executionTime, const float referenceTime)
{
  const float tolerance = 0.025f;
  const float percentualDifference =
    PercentualDifference(executionTime, referenceTime);

  TestsUtils::ExpectValue(percentualDifference, 0.0f, tolerance);
}

// ----------------------------------------------------------------------------

namespace Scenarios {
void
ImmediateTrianglesDraw()
{
  constexpr size_t N_FRAMES = 2000;
  constexpr size_t N_TRIANGLES = 10000;

  ExtrasUtils::Tic();
  DrawTriangles(N_TRIANGLES, N_FRAMES);
  const float execTime = ExtrasUtils::Toc();

  constexpr float EXPECTED_EXECUTION_TIME = 5.526;
  ExpectExecutionTime(execTime, EXPECTED_EXECUTION_TIME);
}

void
RetainedTrianglesDraw()
{
  constexpr size_t N_FRAMES = 100000;
  constexpr size_t N_TRIANGLES = 10000;

  ExtrasUtils::Tic();
  AddAndRemoveTriangles(N_TRIANGLES, N_FRAMES);
  const float execTime = ExtrasUtils::Toc();

  constexpr float EXPECTED_EXECUTION_TIME = 10.894;
  ExpectExecutionTime(execTime, EXPECTED_EXECUTION_TIME);
}
}

// ----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #7");

  TestsUtils::InitTesting(argv, win);
  ExtrasUtils::InitRandom();

  Chimia::Draw3D::Initialize();

  Scenarios::ImmediateTrianglesDraw();
  Scenarios::RetainedTrianglesDraw();

  return 0;
}

// ----------------------------------------------------------------------------