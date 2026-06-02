#include "Draw3D/Camera.h"
#include "Draw3D/Config.h"
#include "Draw3D/Draw3D.h"
#include "Draw3D/Illumination.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"

#include "Media/Image.h"

#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

// ----------------------------------------------------------------------------

std::vector<Chimia::Draw3D::VertexPNT>
FromFloats(const std::vector<float>& floats)
{
  auto calculateNormal = [](const glm::vec3& p) {
    const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
    return p - zero;
  };

  std::vector<Chimia::Draw3D::VertexPNT> vertices;
  for (size_t i = 0; i < floats.size(); i += 8) {
    vertices.push_back(Chimia::Draw3D::VertexPNT{
      { floats[i], floats[i + 1], floats[i + 2] },
      { floats[i + 3], floats[i + 4], floats[i + 5] },
      { floats[i + 6], floats[i + 7] } });

    auto& v = vertices.back();
    v.normal = calculateNormal(v.position);
  }

  return vertices;
}

void
DrawLitCube(const Chimia::Draw3D::TextureID& texture)
{

  // Data extracted from example:
  // https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/4.1.lighting_maps_diffuse_map/lighting_maps_diffuse.cpp
  auto v = FromFloats(
    { -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
      0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
      0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
      -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
      0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
      -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
      -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
      1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
      -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f });

  Chimia::Draw3D::Triangles(v, texture);
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

// ----------------------------------------------------------------------------

Chimia::Draw3D::TextureID
CreateTextureFromImage(const std::string& imagePath)
{
  Chimia::Media::Image texData(imagePath.c_str());

  const Chimia::Draw3D::TextureID texture = Chimia::Draw3D::CreateTexture(
    texData.RawData(), texData.Width(), texData.Height());

  return texture;
}

// ----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
  Window w(1280, 960, "Draw3D ex18");

  Chimia::Draw3D::Initialize();

  Chimia::Draw3D::Config::SetIlluminationModel(
    Chimia::Draw3D::eIlluminationModel::PHONG);

  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";

  const std::string boxAsset = assetsDir + "box.jpg";
  const Chimia::Draw3D::TextureID texture1 = CreateTextureFromImage(boxAsset);

  glm::vec3 cameraPos{ 0.0f, 0.0f, -7.0f };
  glm::vec3 lightPos{ 0.0f, 5.0f, -5.0f };

  Chimia::Draw3D::Camera::Projection::SetPerspective(
    45.0f, 1.0f, 0.01f, 100.0f);
  Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

  const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
  const glm::vec3 lightDir = zero - lightPos;

  Chimia::Draw3D::DirectionalLight dLight{
    lightDir,
    { { 0.1f, 0.1f, 0.1f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f } }
  };

  Chimia::Draw3D::EnableLights(true);
  Chimia::Draw3D::SetLight(dLight);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

    DrawLitCube(texture1);
    DrawLight(lightPos, { 1.0f, 1.0f, 1.0f, 1.0f });

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
