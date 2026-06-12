#include "Draw3D/Draw3D.h"
#include "Draw3D/Illumination.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"

#include "Media/Image.h"
#include "Utils/Window.h"

#include "GraphicsTestsData.h"

#include "TestsUtils.h"

#include <memory>

namespace Scenarios {

std::unique_ptr<Chimia::Draw3D::MaterialID> g_referenceMaterial = nullptr;
std::unique_ptr<Chimia::Draw3D::TextureID> g_referenceTexture = nullptr;

void
ConfigureDefaultLightSource()
{
  glm::vec3 lightPos{ 0.0f, 5.0f, -5.0f };

  const glm::vec3 zero{ 0.0f, 0.0f, 0.0f };
  const glm::vec3 lightDir = zero - lightPos;

  Chimia::Draw3D::DirectionalLight dLight{
    lightDir,
    { { 0.2f, 0.2f, 0.2f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f } }
  };

  Chimia::Draw3D::EnableLights(true);
  Chimia::Draw3D::SetLight(dLight);
}

void
CreateReferenceResources()
{
  Scenarios::g_referenceMaterial.reset(
    new Chimia::Draw3D::MaterialID(Chimia::Draw3D::CreateMaterial(
      { 0.0f, 0.0f, 0.2f }, { 0.0f, 0.0f, 0.7f }, { 1.0f, 1.0f, 1.0f }, 32)));

  const std::string assetsDir = TestsUtils::GetTestingDirectory() + "/assets/";
  Chimia::Media::Image texData(assetsDir + "box.jpg");

  Scenarios::g_referenceTexture.reset(
    new Chimia::Draw3D::TextureID(Chimia::Draw3D::CreateTexture(
      texData.RawData(), texData.Width(), texData.Height())));
}

void
Setup()
{
  CreateReferenceResources();
  ConfigureDefaultLightSource();
}

void
VertexColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuads<Chimia::Draw3D::VertexPC>(
    GraphicsTestsData::QuadPC);

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage("test2_vertexColored.png");
}

void
NormalMaterialColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPN>(
    GraphicsTestsData::QuadPN, *g_referenceMaterial);

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage("test2_materialColoredLit.png");
}

void
Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPT>(
    GraphicsTestsData::QuadPT, *g_referenceTexture);

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage("test2_textured.png");
}

void
VertexColored_Lit(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuads<Chimia::Draw3D::VertexPCN>(
    GraphicsTestsData::QuadPCN);

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage("test2_vertexColoredLit.png");
}

void
VertexColored_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCT>(
    GraphicsTestsData::QuadPCT, *g_referenceTexture);

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage("test2_vertexColoredTextured.png");
}

void
VertexColored_Lit_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCNT>(
    GraphicsTestsData::QuadPCNT, *g_referenceTexture);

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage("test2_vertexColoredLitTextured.png");
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #1");

  TestsUtils::InitTesting(argv, win);

  Chimia::Draw3D::Initialize();

  Scenarios::Setup();

  Scenarios::VertexColored(win);
  Scenarios::NormalMaterialColored(win);
  Scenarios::Textured(win);
  Scenarios::VertexColored_Lit(win);
  Scenarios::VertexColored_Textured(win);
  Scenarios::VertexColored_Lit_Textured(win);

  return 0;
}
