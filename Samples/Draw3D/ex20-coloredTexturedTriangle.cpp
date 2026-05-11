#include "Draw3D/Camera.h"
#include "Draw3D/Config.h"
#include "Draw3D/Draw3D.h"
#include "Draw3D/Illumination.h"
#include "Draw3D/ModelRendering.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"
#include <imgui.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

// ----------------------------------------------------------------------------

namespace Input {
// clang-format off
    const std::vector<float> vertexData {
      // x      y      z    r     g     b    nx    ny     nz    u     v
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.2f, 0.8f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f, 0.0f, 7.0f, 4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    const size_t nVertices = 5;
  
    const std::vector<unsigned> indices {
      0, 1, 2, 0, 3, 4
    };
  
    const glm::mat4x4 transform1{
      0.2f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.2f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.2f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };
    
    const glm::mat4x4 transform2{
      0.2f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.2f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.2f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f
    };
    
    const glm::mat4x4 transform3{
      0.4f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.8f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.2f, 0.0f,
      0.5f, 0.0f, 0.0f, 1.0f
    };
// clang-format on

}

// ----------------------------------------------------------------------------

Chimia::Draw3D::TextureID
CreateTextureFromImage(const std::string& imagePath)
{
  int width, height, _nChannels;
  unsigned char* texData =
    stbi_load(imagePath.c_str(), &width, &height, &_nChannels, 0);

  const Chimia::Draw3D::TextureID texture =
    Chimia::Draw3D::CreateTexture(texData, width, height);

  stbi_image_free(texData);

  return texture;
}

// ----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
  Window w(1280, 960, "Draw3D ex20");

  Chimia::Draw3D::Initialize();

  Chimia::Draw3D::Config::SetIlluminationModel(
    Chimia::Draw3D::eIlluminationModel::PHONG);

  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";

  const std::string boxAsset = assetsDir + "box.jpg";
  const Chimia::Draw3D::TextureID texture1 = CreateTextureFromImage(boxAsset);

  const std::string solarFlareAsset = assetsDir + "solar-flare.jpg";
  const Chimia::Draw3D::TextureID texture2 =
    CreateTextureFromImage(solarFlareAsset);

  glm::vec3 cameraPos{ 0.0f, 0.0f, -2.0f };
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

  const glm::vec3 standardNormal{ 0.0f, 0.0f, -1.0f };

  const auto t1 = Chimia::Draw3D::AddRetainedTriangles(
    { Chimia::Draw3D::VertexPCNT{ { -1.0f, -1.0f, 0.0f },
                                  { 1.0f, 0.0f, 0.0f },
                                  standardNormal,
                                  { 0.0f, 0.0f } },
      Chimia::Draw3D::VertexPCNT{ { -0.5f, -1.0f, 0.0f },
                                  { 0.0f, 1.0f, 0.0f },
                                  standardNormal,
                                  { 0.8f, 0.0f } },
      Chimia::Draw3D::VertexPCNT{ { -0.5f, -0.5f, 0.0f },
                                  { 0.0f, 0.0f, 1.0f },
                                  standardNormal,
                                  { 0.8f, 1.0f } } },
    texture2);

  const auto model = Chimia::Draw3D::CreateModel(
    { Input::vertexData, Input::nVertices, Input::indices },
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR3_NORMAL3_TEXCOORD2);

  const auto instance1 =
    Chimia::Draw3D::AddRetainedModel(model, Input::transform2, texture1);
  const auto instance2 =
    Chimia::Draw3D::AddRetainedModel(model, Input::transform1, texture2);

  SamplesUtils::DoAfterSync(
    [&]() { Chimia::Draw3D::DeleteRetainedTriangles(t1); }, 1000);
  SamplesUtils::DoAfterSync(
    [&]() { Chimia::Draw3D::DeleteRetainedModel(instance1); }, 2000);
  SamplesUtils::DoAfterSync(
    [&]() {
      Chimia::Draw3D::AddRetainedModel(model, Input::transform2, texture1);
    },
    3000);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::Camera::View::LookAt(cameraPos, { 0.0f, 0.0f, 0.0f });

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();
    SamplesUtils::PollDeferredActions();
  }
  return 0;
}

// ----------------------------------------------------------------------------
