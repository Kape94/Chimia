#include "Draw3D/Draw3D.h"
#include "Draw3D/Resources.h"
#include "Draw3D/TransitionRendering.h"
#include "Draw3D/Types.h"
#include "Utils/Window.h"

#include "Utils/Window.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

// clang-format off
const std::vector<float> vertex{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                                 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                                 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };

const std::vector<float> vertex_target{ 0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.3f, 
                                        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.3f, 0.3f,
                                        0.0f, 0.0f, 0.0f, 0.5f, 0.3f, 1.0f, 0.3f };
// clang-format on

const std::vector<unsigned> indexData{ 0, 1, 2 };

int
main()
{
  Window win(1280, 1080, "Example #25");

  Chimia::Draw3D::Initialize();

  auto startModel = Chimia::Draw3D::CreateModel(
    { vertex_target, 3 /*nVertices*/, indexData },
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4);

  auto targetModel = Chimia::Draw3D::CreateModel(
    { vertex, 3 /*nVertices*/, indexData },
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4);

  auto transition = Chimia::Draw3D::CreateTransition(startModel, targetModel);

  constexpr float SPEED = 0.002f;
  float interpolationRate = 0.0f;
  float increment = SPEED;

  const glm::mat4x4 identity = glm::identity<glm::mat4x4>();

  while (!win.ShouldClose()) {
    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::DrawTransition(transition, interpolationRate, identity);

    Chimia::Draw3D::DrawTransition(
      transition,
      interpolationRate,
      glm::translate(identity, { -0.5f, 0.0f, 0.0f }));

    Chimia::Draw3D::DrawTransition(
      transition,
      interpolationRate,
      glm::translate(identity, { -0.5f, -0.3f, 0.0f }));

    Chimia::Draw3D::Flush();

    win.Swap();
    win.PollEvents();

    if (interpolationRate < 0.0f) {
      interpolationRate = 0.01f;
      increment = SPEED;
    } else if (interpolationRate > 1.0f) {
      interpolationRate = 0.99f;
      increment = -SPEED;
    }
    interpolationRate += increment;
  }

  return 0;
}
