#include "Draw3D/Draw3D.h"
#include "Draw3D/Renderers.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

// ----------------------------------------------------------------------------

void
DrawQuad(const glm::vec3& p1,
         const glm::vec3& color1,
         const glm::vec3& p2,
         const glm::vec3& color2,
         const glm::vec3& p3,
         const glm::vec3& color3,
         const glm::vec3& p4,
         const glm::vec3& color4)
{
  // clang-format off
  const std::vector<float> vertexData{
    p1.x, p1.y, p1.z, color1.r, color1.g, color1.b,
    p2.x, p2.y, p2.z, color2.r, color2.g, color2.b,
    p3.x, p3.y, p3.z, color3.r, color3.g, color3.b,
    p4.x, p4.y, p4.z, color4.r, color4.g, color4.b,
  };
  // clang-format on

  static auto& renderer = Chimia::Draw3D::GetVertexColoredRenderer();
  renderer.DrawIndexedTriangles(vertexData, { 0, 1, 2, 2, 3, 0 });
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex7");

  Chimia::Draw3D::Initialize();

  auto& renderer = Chimia::Draw3D::GetVertexColoredRenderer();

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    DrawQuad({ -0.5f, -0.5f, 0.0f },
             { 1.0f, 0.0f, 0.0f },
             { 0.5f, -0.5f, 0.0f },
             { 0.0f, 1.0f, 0.0f },
             { 0.5f, 0.5f, 0.0f },
             { 0.0f, 0.0f, 1.0f },
             { -0.5f, 0.5f, 0.0f },
             { 1.0f, 0.0f, 1.0f });

    DrawQuad({ -1.0f, -1.0f, 0.0f },
             { 1.0f, 0.0f, 0.0f },
             { 0.0f, -1.0f, 0.0f },
             { 0.0f, 1.0f, 0.0f },
             { 0.0f, 1.0f, 0.0f },
             { 0.0f, 0.0f, 1.0f },
             { -1.0f, 1.0f, 0.0f },
             { 1.0f, 0.0f, 1.0f });

    // clang-format off
    renderer.DrawIndexedTriangles({ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
                                  { 0, 1, 2 });
    // clang-format on

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    SamplesUtils::SyncForTargetFPS(1);
  }
  return 0;
}

// ----------------------------------------------------------------------------
