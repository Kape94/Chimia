#include "Draw3D/Draw3D.h"
#include "Draw3D/ModelRendering.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

#include <glm/mat4x4.hpp>

// ----------------------------------------------------------------------------

namespace Input {
// clang-format off
std::vector<float> vertexData{
  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 
  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 
  1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
  0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 
  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
  -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 
};


const size_t nVertices = 6;

std::vector<unsigned> indices{ 0, 1, 2, 3, 4, 5 };

const glm::mat4x4 transform1{ 0.2f, 0.0f, 0.0f, 0.0f, 
                              0.0f, 0.2f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.2f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f };

const glm::mat4x4 transform2{ 0.2f, 0.0f, 0.0f, 0.0f, 
                              0.0f, 0.2f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.2f, 0.0f,
                              0.5f, 0.5f, 0.0f, 1.0f };
// clang-format on
}

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
  Window w(1280, 960, "Draw3D ex17");

  Chimia::Draw3D::Initialize();

  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";

  const std::string blueLightAsset = assetsDir + "blue-light-style.jpg";
  const Chimia::Draw3D::TextureID texture1 =
    CreateTextureFromImage(blueLightAsset);

  const std::string solarFlareAsset = assetsDir + "solar-flare.jpg";
  const Chimia::Draw3D::TextureID texture2 =
    CreateTextureFromImage(solarFlareAsset);

  const auto model = Chimia::Draw3D::CreateModel(
    { Input::vertexData, Input::nVertices, Input::indices },
    Chimia::Draw3D::eVertexLayout::POSITION3_TEXCOORD2);

  const auto triangleStatic = Chimia::Draw3D::AddStaticTriangles(
    { Chimia::Draw3D::VertexPT{ { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
      Chimia::Draw3D::VertexPT{ { 0.5f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
      Chimia::Draw3D::VertexPT{ { 0.5f, -0.7f, 0.0f }, { 1.0f, 1.0f } } },
    { 0, 1, 2 },
    texture2);

  const auto modelInstance1 =
    Chimia::Draw3D::AddStaticModel(model, Input::transform1, texture1);

  SamplesUtils::DoAfter(
    [&]() { Chimia::Draw3D::DeleteStaticModel(modelInstance1); }, 1000);

  SamplesUtils::DoAfter(
    [&]() { Chimia::Draw3D::DeleteStaticTriangles(triangleStatic); }, 2000);

  SamplesUtils::DoAfter(
    [&]() {
      Chimia::Draw3D::AddStaticModel(model, Input::transform1, texture1);
    },
    3000);

  while (!w.ShouldClose()) {
    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::DrawModel(model, Input::transform2, texture2);

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();
  }
  return 0;
}

// ----------------------------------------------------------------------------
