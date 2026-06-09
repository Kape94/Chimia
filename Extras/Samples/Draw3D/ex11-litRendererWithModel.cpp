#include "Draw3D/Camera.h"
#include "Draw3D/Draw3D.h"
#include "Draw3D/Illumination.h"
#include "Draw3D/ModelRendering.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

// ----------------------------------------------------------------------------

struct ModelData
{
  std::vector<float> vertexData;
  size_t nVertices;
  std::vector<unsigned> indexData;
};

void
AppendPointInVertexData(std::vector<float>& vertexData, const glm::vec3& point)
{
  auto normal = [](const glm::vec3& v) {
    const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
    return v - zero;
  };

  const glm::vec3 n = normal(point);
  vertexData.insert(vertexData.end(),
                    { point.x, point.y, point.z, n.x, n.y, n.z });
}

ModelData
CreateCubeGeometry()
{
  const float size = 0.2f;

  const glm::vec3 p1{ -size, -size, -size };
  const glm::vec3 p2{ size, -size, -size };
  const glm::vec3 p3{ size, size, -size };
  const glm::vec3 p4{ -size, size, -size };

  const glm::vec3 p5{ -size, -size, size };
  const glm::vec3 p6{ size, -size, size };
  const glm::vec3 p7{ size, size, size };
  const glm::vec3 p8{ -size, size, size };

  ModelData modelData;
  AppendPointInVertexData(modelData.vertexData, p1);
  AppendPointInVertexData(modelData.vertexData, p2);
  AppendPointInVertexData(modelData.vertexData, p3);
  AppendPointInVertexData(modelData.vertexData, p4);
  AppendPointInVertexData(modelData.vertexData, p5);
  AppendPointInVertexData(modelData.vertexData, p6);
  AppendPointInVertexData(modelData.vertexData, p7);
  AppendPointInVertexData(modelData.vertexData, p8);

  modelData.nVertices = 8;
  // clang-format off
  modelData.indexData.assign({
    0, 1, 2, 2, 3, 0,
    1, 5, 6, 6, 2, 1,
    5, 4, 7, 7, 6, 5,
    4, 0, 3, 3, 7, 4,
    3, 2, 6, 6, 7, 3,
    4, 5, 1, 1, 0, 4
  });
  // clang-format on

  return modelData;
}

void
DrawUnlitTriangle(const glm::vec3& p1,
                  const glm::vec3& p2,
                  const glm::vec3& p3,
                  const glm::vec4& color)
{
  Chimia::Draw3D::Triangle(Chimia::Draw3D::VertexPC{ p1, color },
                           Chimia::Draw3D::VertexPC{ p2, color },
                           Chimia::Draw3D::VertexPC{ p3, color });
}

void
DrawLight(const glm::vec3& lightPos, const glm::vec4& lightColor)
{
  const float size = 0.3f;

  const glm::vec3 p1(lightPos + glm::vec3{ -size, 0.0f, -size });
  const glm::vec3 p2(lightPos + glm::vec3{ size, 0.0f, -size });
  const glm::vec3 p3(lightPos + glm::vec3{ 0.0f, 0.0f, size });
  const glm::vec3 p4(lightPos + glm::vec3{ 0.0f, size, 0.0f });

  DrawUnlitTriangle(p1, p2, p4, lightColor);
  DrawUnlitTriangle(p2, p3, p4, lightColor);
  DrawUnlitTriangle(p3, p1, p4, lightColor);
  DrawUnlitTriangle(p3, p2, p1, lightColor);
}

glm::mat4x4
Transform(const glm::vec3& offset, const float scale)
{
  glm::mat4x4 result = glm::identity<glm::mat4x4>();

  const glm::mat4x4 t = glm::translate(glm::identity<glm::mat4x4>(), offset);
  const glm::mat4x4 s =
    glm::scale(glm::identity<glm::mat4x4>(), { scale, scale, scale });

  result = s * t * result;
  return result;
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex11");

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

  Chimia::Draw3D::EnableLights(true);
  Chimia::Draw3D::SetLight(dLight);

  Chimia::Draw3D::MaterialID blueMaterial = Chimia::Draw3D::CreateMaterial(
    { 0.0f, 0.0f, 0.2f }, { 0.0f, 0.0f, 0.7f }, { 1.0f, 1.0f, 1.0f }, 32);
  Chimia::Draw3D::MaterialID redMaterial = Chimia::Draw3D::CreateMaterial(
    { 0.2f, 0.0f, 0.0f }, { 0.7f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 32);

  ModelData modelData = CreateCubeGeometry();
  const Chimia::Draw3D::ModelID cubeModel = Chimia::Draw3D::CreateModel(
    { modelData.vertexData, modelData.nVertices, modelData.indexData },
    Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3);

  const Chimia::Draw3D::ModelInstanceID staticRedCube =
    Chimia::Draw3D::AddRetainedModel(
      cubeModel, Transform({ 0.5f, 0.8f, 0.0f }, 1.0f), redMaterial);

  ExtrasUtils::DoAfterSync(
    [&]() { Chimia::Draw3D::DeleteRetainedModel(staticRedCube); }, 2000);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

    Chimia::Draw3D::DrawModel(
      cubeModel, Transform({ 0.0f, 0.0f, 0.0f }, 2.0f), blueMaterial);
    Chimia::Draw3D::DrawModel(
      cubeModel, Transform({ 0.5f, 0.0f, 0.0f }, 1.0f), blueMaterial);
    DrawLight(lightPos, { 1.0f, 1.0f, 1.0f, 1.0f });

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    const glm::mat4x4 rotMatrix =
      glm::rotate(glm::identity<glm::mat4x4>(), 0.01f, { 0.0f, 1.0f, 0.0f });
    cameraPos = rotMatrix * glm::vec4(cameraPos, 1.0f);

    ExtrasUtils::PollDeferredActions();
  }
  return 0;
}

// ----------------------------------------------------------------------------
