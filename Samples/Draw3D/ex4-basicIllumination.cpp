#include "Draw3D/Camera.h"
#include "Draw3D/Draw3D.h"
#include "Draw3D/Illumination.h"

#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

// ----------------------------------------------------------------------------

void
DrawTriangle(const glm::vec3& p1,
             const glm::vec3& p2,
             const glm::vec3& p3,
             const Chimia::Draw3D::MaterialID& material)
{

  auto normal = [](const glm::vec3& p) {
    const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
    return p - zero;
  };

  Chimia::Draw3D::LitTriangle(
    p1, normal(p1), p2, normal(p2), p3, normal(p3), material);
}

void
DrawCube(const Chimia::Draw3D::MaterialID& material)
{
  const float size = 1.0f;

  const glm::vec3 p1{ -size, -size, -size };
  const glm::vec3 p2{ size, -size, -size };
  const glm::vec3 p3{ size, size, -size };
  const glm::vec3 p4{ -size, size, -size };

  const glm::vec3 p5{ -size, -size, size };
  const glm::vec3 p6{ size, -size, size };
  const glm::vec3 p7{ size, size, size };
  const glm::vec3 p8{ -size, size, size };

  DrawTriangle(p1, p2, p3, material);
  DrawTriangle(p3, p4, p1, material);

  DrawTriangle(p2, p6, p7, material);
  DrawTriangle(p7, p3, p2, material);

  DrawTriangle(p6, p5, p8, material);
  DrawTriangle(p8, p7, p6, material);

  DrawTriangle(p5, p1, p4, material);
  DrawTriangle(p4, p8, p5, material);

  DrawTriangle(p4, p3, p7, material);
  DrawTriangle(p7, p8, p4, material);

  DrawTriangle(p5, p6, p2, material);
  DrawTriangle(p2, p1, p5, material);
}

void
DrawLight(const glm::vec3& lightPos, const glm::vec3& lightColor)
{
  const float size = 0.3f;

  const glm::vec3 p1(lightPos + glm::vec3{ -size, 0.0f, -size });
  const glm::vec3 p2(lightPos + glm::vec3{ size, 0.0f, -size });
  const glm::vec3 p3(lightPos + glm::vec3{ 0.0f, 0.0f, size });
  const glm::vec3 p4(lightPos + glm::vec3{ 0.0f, size, 0.0f });

  Chimia::Draw3D::Triangle(p1, p2, p4, lightColor);
  Chimia::Draw3D::Triangle(p2, p3, p4, lightColor);
  Chimia::Draw3D::Triangle(p3, p1, p4, lightColor);
  Chimia::Draw3D::Triangle(p3, p2, p1, lightColor);
}

int
main()
{
  Window w(1280, 960, "Draw3D ex4");

  Chimia::Draw3D::Initialize();

  glm::vec3 cameraPos{ 0.0f, 0.0f, -7.0f };
  glm::vec3 lightPos{ 0.0f, 5.0f, -5.0f };

  Chimia::Draw3D::Camera::Projection::SetPerspective(
    45.0f, 1.0f, 0.01f, 100.0f);
  Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

  const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
  const glm::vec3 lightDir = zero - lightPos;

  Chimia::Draw3D::DirectionalLight dLight{
    lightDir,
    { { 0.2f, 0.2f, 0.2f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f } }
  };

  const Chimia::Draw3D::MaterialID material = Chimia::Draw3D::CreateMaterial(
    { 0.0f, 0.0f, 0.2f }, { 0.0f, 0.0f, 0.7f }, { 1.0f, 1.0f, 1.0f }, 32);

  Chimia::Draw3D::EnableLights(true);
  Chimia::Draw3D::SetLight(dLight);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

    DrawCube(material);
    DrawLight(lightPos, { 1.0f, 1.0f, 1.0f });

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    const glm::mat4x4 rotMatrix =
      glm::rotate(glm::identity<glm::mat4x4>(), 0.01f, { 0.0f, 1.0f, 0.0f });
    cameraPos = rotMatrix * glm::vec4(cameraPos, 1.0f);
  }
  return 0;
}

// ----------------------------------------------------------------------------
