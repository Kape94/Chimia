#include "Core/Types.h"
#include "Draw3D/Camera.h"
#include "Draw3D/Config.h"
#include "Draw3D/Draw3D.h"
#include "Draw3D/Illumination.h"

#include "Draw3D/Renderers.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

// ----------------------------------------------------------------------------

auto& renderer = Chimia::Draw3D::GetLitRenderer();
auto& unlitRenderer = Chimia::Draw3D::GetVertexColoredRenderer();

// ----------------------------------------------------------------------------

void
DrawTriangle(const glm::vec3& p1,
             const glm::vec3& p2,
             const glm::vec3& p3,
             const Chimia::Draw3D::MaterialID& material)
{

  auto normal = [](const glm::vec3& v) {
    const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
    return v - zero;
  };

  renderer.DrawTriangle(Chimia::Position3(p1),
                        Chimia::Normal3(normal(p1)),
                        Chimia::Position3(p2),
                        Chimia::Normal3(normal(p2)),
                        Chimia::Position3(p3),
                        Chimia::Normal3(normal(p3)),
                        material);
}

void
DrawLitCube(const Chimia::Draw3D::MaterialID& material)
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
DrawLight(const Chimia::Position3& lightPos, const Chimia::Color3& lightColor)
{
  const float size = 0.3f;

  const glm::vec3& pos = lightPos.AsVec3();
  const Chimia::Position3 p1(pos + glm::vec3{ -size, 0.0f, -size });
  const Chimia::Position3 p2(pos + glm::vec3{ size, 0.0f, -size });
  const Chimia::Position3 p3(pos + glm::vec3{ 0.0f, 0.0f, size });
  const Chimia::Position3 p4(pos + glm::vec3{ 0.0f, size, 0.0f });

  unlitRenderer.DrawTriangle(p1, lightColor, p2, lightColor, p4, lightColor);
  unlitRenderer.DrawTriangle(p2, lightColor, p3, lightColor, p4, lightColor);
  unlitRenderer.DrawTriangle(p3, lightColor, p1, lightColor, p4, lightColor);
  unlitRenderer.DrawTriangle(p3, lightColor, p2, lightColor, p1, lightColor);
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex12");

  Chimia::Draw3D::Initialize();

  Chimia::Draw3D::Config::Lit::SetIlluminationModel(
    Chimia::Draw3D::eIlluminationModel::PHONG);

  glm::vec3 cameraPos{ 0.0f, 0.0f, -7.0f };
  Chimia::Position3 lightPos{ 0.0f, 5.0f, -5.0f };

  Chimia::Draw3D::Camera::Projection::SetPerspective(
    45.0f, 1.0f, 0.01f, 100.0f);
  Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

  const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
  const glm::vec3 lightDir = zero - lightPos.AsVec3();

  Chimia::Draw3D::DirectionalLight dLight{
    lightDir,
    { { 0.2f, 0.2f, 0.2f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f } }
  };

  Chimia::Draw3D::EnableLights(true);
  Chimia::Draw3D::SetLight(dLight);

  Chimia::Draw3D::MaterialID tMaterial = Chimia::Draw3D::CreateMaterial(
    { 0.0f, 0.0f, 0.2f }, { 0.0f, 0.0f, 0.7f }, { 1.0f, 1.0f, 1.0f }, 32);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

    DrawLitCube(tMaterial);
    DrawLight(lightPos, Chimia::Color3{ 1.0f, 1.0f, 1.0f });

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
