#include "Draw3D/Draw3D.h"
#include "Draw3D/ModelRendering.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

// ----------------------------------------------------------------------------

namespace Input {
// clang-format off
  const std::vector<float> vertexData {
    // x      y      z      r     g     b
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
      0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };
  const size_t nVertices = 5;

  const std::vector<unsigned> indices {
    0, 1, 2, 0, 3, 4
  };

  const glm::mat4x4 transform1{
    0.2f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.2f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.2f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
  
  const glm::mat4x4 transform2{
    0.2f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.2f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.2f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f
  };
  
  const glm::mat4x4 transform3{
    0.4f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.8f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.2f, 0.0f,
    0.5f, 0.0f, 0.0f, 1.0f
  };
// clang-format on

}

int
main()
{
  Window w(1280, 960, "Draw3D ex9");

  Chimia::Draw3D::Initialize();

  const Chimia::Draw3D::ModelID modelID = Chimia::Draw3D::CreateModel(
    { Input::vertexData, Input::nVertices, Input::indices },
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR3);

  const Chimia::Draw3D::ModelInstanceID instance1 =
    Chimia::Draw3D::AddStaticModel(modelID, Input::transform1);
  const Chimia::Draw3D::ModelInstanceID instance2 =
    Chimia::Draw3D::AddStaticModel(modelID, Input::transform2);

  auto deleteInstance1 = [&]() {
    Chimia::Draw3D::DeleteStaticModel(instance1);
  };
  auto deleteInstance2 = [&]() {
    Chimia::Draw3D::DeleteStaticModel(instance2);
  };
  auto addInstance1 = [&]() {
    Chimia::Draw3D::AddStaticModel(modelID, Input::transform1);
  };
  SamplesUtils::DoAfterSync(deleteInstance1, 1000);
  SamplesUtils::DoAfterSync(deleteInstance2, 2000);
  SamplesUtils::DoAfterSync(addInstance1, 3000);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::DrawModel(modelID, Input::transform3);

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    SamplesUtils::PollDeferredActions();
    SamplesUtils::SyncForTargetFPS(10);
  }
  return 0;
}

// ----------------------------------------------------------------------------
