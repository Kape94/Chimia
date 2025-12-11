#include "Draw3D/Draw3D.h"
#include "Draw3D/Triangle.h"
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
  const std::vector<Chimia::Draw3D::VertexPC> vertexData{
    // First triangle
    Chimia::Draw3D::VertexPC{ { p1.x, p1.y, p1.z },
                              { color1.r, color1.g, color1.b } },
    Chimia::Draw3D::VertexPC{ { p2.x, p2.y, p2.z },
                              { color2.r, color2.g, color2.b } },
    Chimia::Draw3D::VertexPC{ { p3.x, p3.y, p3.z },
                              { color3.r, color3.g, color3.b } },
    // Second triangle
    Chimia::Draw3D::VertexPC{ { p3.x, p3.y, p3.z },
                              { color3.r, color3.g, color3.b } },
    Chimia::Draw3D::VertexPC{ { p4.x, p4.y, p4.z },
                              { color4.r, color4.g, color4.b } },
    Chimia::Draw3D::VertexPC{ { p1.x, p1.y, p1.z },
                              { color1.r, color1.g, color1.b } },
  };

  return Chimia::Draw3D::AddStaticTriangles(vertexData);
}

Chimia::Draw3D::TriangleMeshID
AddStaticQuadIndexed(const glm::vec3& p1,
                     const glm::vec3& color1,
                     const glm::vec3& p2,
                     const glm::vec3& color2,
                     const glm::vec3& p3,
                     const glm::vec3& color3,
                     const glm::vec3& p4,
                     const glm::vec3& color4)
{
  const std::vector<Chimia::Draw3D::VertexPC> vertexData{
    // First triangle
    Chimia::Draw3D::VertexPC{ { p1.x, p1.y, p1.z },
                              { color1.r, color1.g, color1.b } },
    Chimia::Draw3D::VertexPC{ { p2.x, p2.y, p2.z },
                              { color2.r, color2.g, color2.b } },
    Chimia::Draw3D::VertexPC{ { p3.x, p3.y, p3.z },
                              { color3.r, color3.g, color3.b } },
    Chimia::Draw3D::VertexPC{ { p4.x, p4.y, p4.z },
                              { color4.r, color4.g, color4.b } },
  };

  const std::vector<unsigned> indices{ 0, 1, 2, 2, 3, 0 };

  return Chimia::Draw3D::AddStaticTriangles(vertexData, indices);
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex8");

  Chimia::Draw3D::Initialize();

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
    AddStaticQuadIndexed({ -1.0f, -1.0f, 0.0f },
                         { 1.0f, 0.0f, 0.0f },
                         { 0.0f, -1.0f, 0.0f },
                         { 0.0f, 1.0f, 0.0f },
                         { 0.0f, 1.0f, 0.0f },
                         { 0.0f, 0.0f, 1.0f },
                         { -1.0f, 1.0f, 0.0f },
                         { 1.0f, 0.0f, 1.0f });

  auto deleteQuad1 = [&]() { Chimia::Draw3D::DeleteStaticTriangles(quad1); };
  auto deleteQuad2 = [&]() { Chimia::Draw3D::DeleteStaticTriangles(quad2); };
  auto addQuad = [&]() {
    AddStaticQuadIndexed({ -1.0f, -1.0f, 0.0f },
                         { 1.0f, 0.0f, 1.0f },
                         { 0.0f, -1.0f, 0.0f },
                         { 0.0f, 1.0f, 1.0f },
                         { 0.0f, 1.0f, 0.0f },
                         { 0.0f, 0.0f, 1.0f },
                         { -1.0f, 1.0f, 0.0f },
                         { 1.0f, 0.0f, 1.0f });
  };
  SamplesUtils::DoAfterSync(deleteQuad2, 1000);
  SamplesUtils::DoAfterSync(deleteQuad1, 2000);
  SamplesUtils::DoAfterSync(addQuad, 3000);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    SamplesUtils::PollDeferredActions();
    SamplesUtils::SyncForTargetFPS(10);
  }
  return 0;
}

// ----------------------------------------------------------------------------
