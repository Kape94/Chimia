#include "TestImmediateModeTriangles.h"

#include "Draw3D/Draw3D.h"
#include "Draw3D/Illumination.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"

#include "Media/Image.h"
#include "Utils/Window.h"

#include "GraphicsTestsData.h"

#include "TestsUtils.h"

#include <memory>

// ----------------------------------------------------------------------------

namespace Scenarios {

std::unique_ptr<Chimia::Draw3D::MaterialID> g_referenceMaterial = nullptr;
std::unique_ptr<Chimia::Draw3D::TextureID> g_referenceTexture = nullptr;
ImmediateTrianglesTestInfo g_testInfo;

bool g_isSettedUp = false;

std::string
FullArtifactName(const std::string& artifactName)
{
  return g_testInfo.testName + "_" + artifactName;
}

unsigned
FlushOnEvery()
{
  return g_testInfo.flushOnEvery;
}

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
  if (g_isSettedUp) {
    return;
  }

  CreateReferenceResources();
  ConfigureDefaultLightSource();
  g_isSettedUp = true;
}

void
VertexColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuads<Chimia::Draw3D::VertexPC>(
    GraphicsTestsData::QuadPC, FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(FullArtifactName("vertexColored.png"));
}

void
NormalMaterialColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPN>(
    GraphicsTestsData::QuadPN, *g_referenceMaterial, FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(FullArtifactName("materialColoredLit.png"));
}

void
Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPT>(
    GraphicsTestsData::QuadPT, *g_referenceTexture, FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(FullArtifactName("textured.png"));
}

void
VertexColored_Lit(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuads<Chimia::Draw3D::VertexPCN>(
    GraphicsTestsData::QuadPCN, FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(FullArtifactName("vertexColoredLit.png"));
}

void
VertexColored_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCT>(
    GraphicsTestsData::QuadPCT, *g_referenceTexture, FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(FullArtifactName("vertexColoredTextured.png"));
}

void
VertexColored_Lit_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  GraphicsTestsData::DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCNT>(
    GraphicsTestsData::QuadPCNT, *g_referenceTexture, FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(FullArtifactName("vertexColoredLitTextured.png"));
}

}

// ----------------------------------------------------------------------------

void
TestImmediateModeTriangles(const ImmediateTrianglesTestInfo& testInfo,
                           Window& window)
{
  Scenarios::g_testInfo = testInfo;
  if (Scenarios::g_testInfo.testName.empty()) {
    Scenarios::g_testInfo.testName = "testCommon";
  }

  Scenarios::Setup();

  Scenarios::VertexColored(window);
  Scenarios::NormalMaterialColored(window);
  Scenarios::Textured(window);
  Scenarios::VertexColored_Lit(window);
  Scenarios::VertexColored_Textured(window);
  Scenarios::VertexColored_Lit_Textured(window);
}

// ----------------------------------------------------------------------------