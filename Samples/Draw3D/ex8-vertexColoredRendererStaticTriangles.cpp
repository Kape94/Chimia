#include "Draw3D/Draw3D.h"
#include "Draw3D/Renderers.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

// ----------------------------------------------------------------------------

Chimia::Draw3D::TriangleMeshID
AddStaticQuad(const glm::vec3& p1,
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
    // First triangle
    p1.x, p1.y, p1.z, color1.r, color1.g, color1.b,
    p2.x, p2.y, p2.z, color2.r, color2.g, color2.b,
    p3.x, p3.y, p3.z, color3.r, color3.g, color3.b,
    // Second triangle
    p3.x, p3.y, p3.z, color3.r, color3.g, color3.b,
    p4.x, p4.y, p4.z, color4.r, color4.g, color4.b,
    p1.x, p1.y, p1.z, color1.r, color1.g, color1.b,
  };
  // clang-format on

  static auto& renderer = Chimia::Draw3D::GetVertexColoredRenderer();
  return renderer.AddStaticTriangles(vertexData);
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex8");

  Chimia::Draw3D::Initialize();

  auto& renderer = Chimia::Draw3D::GetVertexColoredRenderer();

  const Chimia::Draw3D::TriangleMeshID quad1 =
    AddStaticQuad({ -0.5f, -0.5f, 0.0f },
                  { 1.0f, 0.0f, 0.0f },
                  { 0.5f, -0.5f, 0.0f },
                  { 0.0f, 1.0f, 0.0f },
                  { 0.5f, 0.5f, 0.0f },
                  { 0.0f, 0.0f, 1.0f },
                  { -0.5f, 0.5f, 0.0f },
                  { 1.0f, 0.0f, 1.0f });

  const Chimia::Draw3D::TriangleMeshID quad2 =
    AddStaticQuad({ -1.0f, -1.0f, 0.0f },
                  { 1.0f, 0.0f, 0.0f },
                  { 0.0f, -1.0f, 0.0f },
                  { 0.0f, 1.0f, 0.0f },
                  { 0.0f, 1.0f, 0.0f },
                  { 0.0f, 0.0f, 1.0f },
                  { -1.0f, 1.0f, 0.0f },
                  { 1.0f, 0.0f, 1.0f });

  auto deleteQuad1 = [&]() { renderer.DeleteStaticTriangles(quad1); };
  auto deleteQuad2 = [&]() { renderer.DeleteStaticTriangles(quad2); };
  auto addQuad = [&]() {
    AddStaticQuad({ -1.0f, -1.0f, 0.0f },
                  { 1.0f, 0.0f, 1.0f },
                  { 0.0f, -1.0f, 0.0f },
                  { 0.0f, 1.0f, 1.0f },
                  { 0.0f, 1.0f, 0.0f },
                  { 0.0f, 0.0f, 1.0f },
                  { -1.0f, 1.0f, 0.0f },
                  { 1.0f, 0.0f, 1.0f });
  };
  SamplesUtils::DoAfter(deleteQuad2, 3000);
  SamplesUtils::DoAfter(deleteQuad1, 5000);
  SamplesUtils::DoAfter(addQuad, 8000);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    SamplesUtils::SyncForTargetFPS(1);
  }
  return 0;
}

// ----------------------------------------------------------------------------
