#include "Draw3D/Draw3D.h"
#include "Draw3D/ModelRendering.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <glm/mat4x4.hpp>

// ----------------------------------------------------------------------------

namespace Input {
// clang-format off
std::vector<float> vertexData{
  //  x      y.      z.       r.     g.     b.     a.    u.    v 
  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
  1.0f,  0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 
  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
  0.0f,  0.0f,  0.0f, 0.2f,  0.0f, 8.0f, 1.0f, 0.0f, 0.0f, 
  -1.0f, 0.0f, 0.0f, 0.0f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 
  -1.0f, -1.0f, 0.0f, 0.4f,  0.7f, 0.5f, 1.0f, 1.0f, 1.0f, 
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
    SamplesUtils::ReadImage(imagePath.c_str(), width, height, _nChannels);

  const Chimia::Draw3D::TextureID texture =
    Chimia::Draw3D::CreateTexture(texData, width, height);

  SamplesUtils::FreeImageData(texData);

  return texture;
}

// ----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
  Window w(1280, 960, "Draw3D ex21");

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
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_TEXCOORD2);

  const auto triangleStatic = Chimia::Draw3D::AddRetainedTriangles(
    { Chimia::Draw3D::VertexPCT{
        { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
      Chimia::Draw3D::VertexPCT{
        { 0.5f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.7f, 1.0f }, { 1.0f, 0.0f } },
      Chimia::Draw3D::VertexPCT{
        { 0.5f, -0.7f, 0.0f }, { 0.2f, 0.3f, 0.8f, 1.0f }, { 1.0f, 1.0f } } },
    { 0, 1, 2 },
    texture2);

  const auto modelInstance1 =
    Chimia::Draw3D::AddRetainedModel(model, Input::transform1, texture1);

  SamplesUtils::DoAfterSync(
    [&]() { Chimia::Draw3D::DeleteRetainedModel(modelInstance1); }, 1000);

  SamplesUtils::DoAfterSync(
    [&]() { Chimia::Draw3D::DeleteRetainedTriangles(triangleStatic); }, 2000);

  SamplesUtils::DoAfterSync(
    [&]() {
      Chimia::Draw3D::AddRetainedModel(model, Input::transform1, texture1);
    },
    3000);

  while (!w.ShouldClose()) {
    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::DrawModel(model, Input::transform2, texture2);

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();
    SamplesUtils::PollSingleDeferredAction();
  }
  return 0;
}

// ----------------------------------------------------------------------------
