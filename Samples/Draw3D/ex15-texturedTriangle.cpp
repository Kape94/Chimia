#include "Draw3D/Draw3D.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

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
  Window w(1280, 960, "Draw3D ex15");

  Chimia::Draw3D::Initialize();

  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";

  const std::string blueLightAsset = assetsDir + "blue-light-style.jpg";
  const Chimia::Draw3D::TextureID texture1 =
    CreateTextureFromImage(blueLightAsset);

  const std::string solarFlareAsset = assetsDir + "solar-flare.jpg";
  const Chimia::Draw3D::TextureID texture2 =
    CreateTextureFromImage(solarFlareAsset);

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPT{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
      Chimia::Draw3D::VertexPT{ { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
      Chimia::Draw3D::VertexPT{ { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
      texture1);

    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPT{ { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
      Chimia::Draw3D::VertexPT{ { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
      Chimia::Draw3D::VertexPT{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
      texture2);

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();
  }
  return 0;
}

// ----------------------------------------------------------------------------
