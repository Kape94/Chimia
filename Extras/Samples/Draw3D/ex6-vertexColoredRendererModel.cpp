#include "Draw3D/Draw3D.h"
#include "Draw3D/ModelRendering.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"

#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

// ----------------------------------------------------------------------------

namespace Inputs {

// clang-format off
std::vector<float> vertexData{
  //  x          y          z         r.        g         b         a
  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
  -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
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

  const Chimia::Draw3D::ModelID modelID = Chimia::Draw3D::CreateModel(
    { Inputs::vertexData, Inputs::nVertices, Inputs::indices },
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::DrawModel(modelID, Inputs::transform1);
    Chimia::Draw3D::DrawModel(modelID, Inputs::transform2);
    Chimia::Draw3D::DrawModel(modelID, Inputs::transform3);

    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPC{ { -1.0f, -1.0f, 0.0f },
                                { 1.0f, 0.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { -1.0f, -0.5f, 0.0f },
                                { 0.0f, 1.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { -0.5f, -0.5f, 0.0f },
                                { 0.0f, 0.0f, 1.0f, 1.0f } });

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    ExtrasUtils::SyncForTargetFPS(10);
  }
  return 0;
}

// ----------------------------------------------------------------------------
