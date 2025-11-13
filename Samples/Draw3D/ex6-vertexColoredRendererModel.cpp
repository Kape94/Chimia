#include "Draw3D/Draw3D.h"
#include "Draw3D/Renderers.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

// ----------------------------------------------------------------------------

namespace Inputs {

// clang-format off
std::vector<float> vertexData{
  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
  -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
};
const size_t nVertices = 6;

std::vector<unsigned> indices{ 0, 1, 2, 3, 4, 5 };

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
  0.5f, 0.5f, 0.0f, 1.0f
};

const glm::mat4x4 transform3{
  0.4f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.2f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.2f, 0.0f,
  0.5f, 0.0f, 0.0f, 1.0f
};
// clang-format on

}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex6");

  Chimia::Draw3D::Initialize();

  auto& renderer = Chimia::Draw3D::GetVertexColoredRenderer();

  const Chimia::Draw3D::ModelID modelID = Chimia::Draw3D::CreateModel(
    { Inputs::vertexData, Inputs::nVertices, Inputs::indices });

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    renderer.DrawModelTransformed(modelID, Inputs::transform1);
    renderer.DrawModelTransformed(modelID, Inputs::transform2);
    renderer.DrawModelTransformed(modelID, Inputs::transform3);

    // clang-format off
    renderer.DrawTriangles({ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                            -1.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f });
    // clang-format on

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    SamplesUtils::SyncForTargetFPS(1);
  }
  return 0;
}

// ----------------------------------------------------------------------------
