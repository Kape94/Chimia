#include "Draw3D/Config.h"
#include "Draw3D/Draw3D.h"

#include "Draw3D/Types.h"
#include "Utils/Window.h"

#include "TestTrianglesDrawing.h"

#include "TestsUtils.h"

namespace Scenarios {

std::vector<Chimia::Draw3D::BatchingSettings*>
AllBatchingSettings()
{
  auto& modelBatching =
    Chimia::Draw3D::Config::Batching::ModelBatchingSettings();
  auto& modelBatchingByResource =
    Chimia::Draw3D::Config::Batching::ModelBatchingByResourceSettings();

  auto& triangleBatching =
    Chimia::Draw3D::Config::Batching::TriangleBatchingSettings();
  auto& triangleBatchingByResource =
    Chimia::Draw3D::Config::Batching::TriangleBatchingByResourceSettings();

  std::vector<Chimia::Draw3D::BatchingSettings*> settings;
  return { &modelBatching,
           &modelBatchingByResource,
           &triangleBatching,
           &triangleBatchingByResource };
}

void
SetBatchSizes(unsigned initialSize, unsigned maximumSize)
{
  for (Chimia::Draw3D::BatchingSettings* setting : AllBatchingSettings()) {
    setting->initialBatchSize = initialSize;
    setting->maximumBatchSize = maximumSize;
  }
}

TrianglesDrawingTestInfo
TestWithoutFlush(const std::string& testName)
{
  return { testName, 1000 };
}

TrianglesDrawingTestInfo
TestWithFlush(const std::string& testName, const unsigned flushOnEvery)
{
  return { testName, flushOnEvery };
}

void
BatchSize1Immediate(Window& win)
{
  SetBatchSizes(1, 1);

  Chimia::Draw3D::Restart();

  TestImmediateModeTriangles(TestWithoutFlush("test3_batchSize1"), win);
}

void
BatchSize3Immediate(Window& win)
{
  SetBatchSizes(3, 3);

  Chimia::Draw3D::Restart();

  TestImmediateModeTriangles(TestWithoutFlush("test3_batchSize3"), win);
}

void
ElasticBatchImmediate(Window& win)
{
  SetBatchSizes(1, 100);

  Chimia::Draw3D::Restart();

  TestImmediateModeTriangles(TestWithFlush("test3_batchSizeElastic", 2), win);
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #3");

  TestsUtils::InitTesting(argv, win);

  Chimia::Draw3D::Initialize();

  Scenarios::BatchSize1Immediate(win);
  Scenarios::BatchSize3Immediate(win);
  Scenarios::ElasticBatchImmediate(win);

  return 0;
}
