#include "Draw3D/Draw3D.h"

#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

// ----------------------------------------------------------------------------

void
DrawQuad(const glm::vec3& p1,
         const glm::vec4& color1,
         const glm::vec3& p2,
         const glm::vec4& color2,
         const glm::vec3& p3,
         const glm::vec4& color3,
         const glm::vec3& p4,
         const glm::vec4& color4)
{
  const std::vector<Chimia::Draw3D::VertexPC> vertexData{
    Chimia::Draw3D::VertexPC{ { p1.x, p1.y, p1.z },
                              { color1.r, color1.g, color1.b, color1.a } },
    Chimia::Draw3D::VertexPC{ { p2.x, p2.y, p2.z },
                              { color2.r, color2.g, color2.b, color2.a } },
    Chimia::Draw3D::VertexPC{ { p3.x, p3.y, p3.z },
                              { color3.r, color3.g, color3.b, color3.a } },
    Chimia::Draw3D::VertexPC{ { p4.x, p4.y, p4.z },
                              { color4.r, color4.g, color4.b, color4.a } },
  };

  Chimia::Draw3D::Triangles(vertexData, { 0, 1, 2, 2, 3, 0 });
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex7");

  Chimia::Draw3D::Initialize();

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    DrawQuad({ -0.5f, -0.5f, 0.0f },
             { 1.0f, 0.0f, 0.0f, 1.0f },
             { 0.5f, -0.5f, 0.0f },
             { 0.0f, 1.0f, 0.0f, 1.0f },
             { 0.5f, 0.5f, 0.0f },
             { 0.0f, 0.0f, 1.0f, 1.0f },
             { -0.5f, 0.5f, 0.0f },
             { 1.0f, 0.0f, 1.0f, 1.0f });

    DrawQuad({ -1.0f, -1.0f, 0.0f },
             { 1.0f, 0.0f, 0.0f, 1.0f },
             { 0.0f, -1.0f, 0.0f },
             { 0.0f, 1.0f, 0.0f, 1.0f },
             { 0.0f, 1.0f, 0.0f },
             { 0.0f, 0.0f, 1.0f, 1.0f },
             { -1.0f, 1.0f, 0.0f },
             { 1.0f, 0.0f, 1.0f, 1.0f });

    Chimia::Draw3D::Triangles(
      { Chimia::Draw3D::VertexPC{ { 0.0f, 0.0f, 0.0f },
                                  { 1.0f, 0.0f, 0.0f, 1.0f } },
        Chimia::Draw3D::VertexPC{ { 1.0f, 0.0f, 0.0f },
                                  { 0.0f, 1.0f, 0.0f, 1.0f } },
        Chimia::Draw3D::VertexPC{ { 1.0f, 1.0f, 0.0f },
                                  { 0.0f, 0.0f, 1.0f, 1.0f } } },
      { 0, 1, 2 });

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    SamplesUtils::SyncForTargetFPS(10);
  }
  return 0;
}

// ----------------------------------------------------------------------------
