#include "Draw3D/Camera.h"
#include "Draw3D/Draw3D.h"
#include "Draw3D/Triangle.h"

#include "Draw3D/Types.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex3");

  Chimia::Draw3D::Initialize();

  const float triangleSize = 1.0f;
  const float vertexSize = triangleSize / 2.0f;

  glm::vec3 cameraPos{ 0.0f, 1.0f, -2.0f };

  Chimia::Draw3D::Camera::Projection::SetPerspective(
    45.0f, 1.0f, 0.01f, 100.0f);
  Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

  while (!w.ShouldClose()) {
    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });
    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPC{ { -vertexSize, -vertexSize, 0.0f },
                                { 1.0f, 0.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { vertexSize, -vertexSize, 0.0f },
                                { 0.0f, 1.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { -vertexSize, vertexSize, 0.0f },
                                { 0.0f, 0.0f, 1.0f, 1.0f } });

    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPC{ { vertexSize, -vertexSize, 0.0f },
                                { 1.0f, 0.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { vertexSize, vertexSize, 0.0f },
                                { 0.0f, 1.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { -vertexSize, vertexSize, 0.0f },
                                { 0.0f, 0.0f, 1.0f, 1.0f } });

    Chimia::Draw3D::Flush();

    const glm::mat4x4 rotMatrix =
      glm::rotate(glm::identity<glm::mat4x4>(), 0.01f, { 0.0f, 1.0f, 0.0f });
    cameraPos = rotMatrix * glm::vec4(cameraPos, 1.0f);

    w.Swap();
    w.PollEvents();
  }
  return 0;
}

// ----------------------------------------------------------------------------
