#include "Draw3D/Config.h"
#include "Draw3D/Draw3D.h"

#include "Draw3D/Types.h"
#include "QuadsDrawingFixture.h"
#include "TestModelsDrawing.h"
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

CommonTestingConfig
TestImmediateWithoutFlush(const std::string& testName)
{
  return { testName, 1000 };
}

CommonTestingConfig
TestImmediateWithFlush(const std::string& testName, const unsigned flushOnEvery)
{
  return { testName, flushOnEvery };
}

CommonTestingConfig
TestRetained(const std::string& testName)
{
  /*In this case we're interested in testing only the rendering after the
   * retained quads are added, that's why we request the test to not verify
   * (take screenshots and assert) the screen after removing the retained
   * elements, and also specify the flushOnEvery in a way that it just takes a
   * single screenshot after we add all the quads*/
  const unsigned flushOnEvery = QuadsDrawingFixture::NQuads();
  const bool shouldVerifyRetainedRemovals = false;
  return { testName, flushOnEvery, shouldVerifyRetainedRemovals };
}

void
BatchSize1Immediate(Window& win)
{
  SetBatchSizes(1, 1);

  Chimia::Draw3D::Restart();

  TestImmediateModeTriangles(TestImmediateWithoutFlush("test3_batchSize1"),
                             win);
}

void
BatchSize3Immediate(Window& win)
{
  SetBatchSizes(3, 3);

  Chimia::Draw3D::Restart();

  TestImmediateModeTriangles(TestImmediateWithoutFlush("test3_batchSize3"),
                             win);
}

void
ElasticBatchImmediate(Window& win)
{
  SetBatchSizes(1, 100);

  Chimia::Draw3D::Restart();

  TestImmediateModeTriangles(
    TestImmediateWithFlush("test3_batchSizeElastic", 2), win);
}

void
BatchSize1Retained(Window& win)
{
  SetBatchSizes(1, 1);

  Chimia::Draw3D::Restart();

  TestRetainedModeTriangles(TestRetained("test3_batchSize1"), win);
}

void
BatchSize3Retained(Window& win)
{
  SetBatchSizes(3, 3);

  Chimia::Draw3D::Restart();

  TestRetainedModeTriangles(TestRetained("test3_batchSize3"), win);
}

void
ElasticBatchRetained(Window& win)
{
  SetBatchSizes(1, 100);

  Chimia::Draw3D::Restart();

  TestRetainedModeTriangles(TestRetained("test3_batchSizeElastic"), win);
}

void
BatchSize1ModelsImmediate(Window& win)
{
  SetBatchSizes(1, 1);

  Chimia::Draw3D::Restart();

  TestImmediateModeModels(TestImmediateWithoutFlush("test3_batchSize1_models"),
                          win);
}

void
BatchSize3ModelsImmediate(Window& win)
{
  SetBatchSizes(3, 3);

  Chimia::Draw3D::Restart();

  TestImmediateModeModels(TestImmediateWithoutFlush("test3_batchSize3_models"),
                          win);
}

void
ElasticBatchModelsImmediate(Window& win)
{
  SetBatchSizes(1, 100);

  Chimia::Draw3D::Restart();

  TestImmediateModeModels(
    TestImmediateWithFlush("test3_batchSizeElastic_models", 2), win);
}

void
BatchSize1ModelsRetained(Window& win)
{
  SetBatchSizes(1, 1);

  Chimia::Draw3D::Restart();

  TestRetainedModeModels(TestRetained("test3_batchSize1_models"), win);
}

void
BatchSize3ModelsRetained(Window& win)
{
  SetBatchSizes(3, 3);

  Chimia::Draw3D::Restart();

  TestRetainedModeModels(TestRetained("test3_batchSize3_models"), win);
}

void
ElasticBatchModelsRetained(Window& win)
{
  SetBatchSizes(1, 100);

  Chimia::Draw3D::Restart();

  TestRetainedModeModels(TestRetained("test3_batchSizeElastic_models"), win);
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
  Scenarios::BatchSize1Retained(win);
  Scenarios::BatchSize3Retained(win);
  Scenarios::ElasticBatchRetained(win);

  Scenarios::BatchSize1ModelsImmediate(win);
  Scenarios::BatchSize3ModelsImmediate(win);
  Scenarios::ElasticBatchModelsImmediate(win);
  Scenarios::BatchSize1ModelsRetained(win);
  Scenarios::BatchSize3ModelsRetained(win);
  Scenarios::ElasticBatchModelsRetained(win);

  return 0;
}
