#include "QuadsDrawingTest.h"

#include "Draw3D/Illumination.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"

#include "Media/Image.h"

#include "TestsUtils.h"

#include <memory>

// ----------------------------------------------------------------------------

namespace Internal {

std::unique_ptr<Chimia::Draw3D::MaterialID> g_referenceMaterial = nullptr;
std::unique_ptr<Chimia::Draw3D::TextureID> g_referenceTexture = nullptr;
ImmediateTrianglesTestInfo g_testInfo;

bool g_isSettedUp = false;

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
  g_referenceMaterial.reset(
    new Chimia::Draw3D::MaterialID(Chimia::Draw3D::CreateMaterial(
      { 0.0f, 0.0f, 0.2f }, { 0.0f, 0.0f, 0.7f }, { 1.0f, 1.0f, 1.0f }, 32)));

  const std::string assetsDir = TestsUtils::GetTestingDirectory() + "/assets/";
  Chimia::Media::Image texData(assetsDir + "box.jpg");

  g_referenceTexture.reset(
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

}

// ----------------------------------------------------------------------------

void
QuadsDrawingTest::Init(const ImmediateTrianglesTestInfo& testInfo)
{
  Internal::g_testInfo = testInfo;
  if (Internal::g_testInfo.testName.empty()) {
    Internal::g_testInfo.testName = "testCommon";
  }

  Internal::Setup();
}

// ----------------------------------------------------------------------------

std::string
QuadsDrawingTest::FullArtifactName(const std::string& artifactName)
{
  return Internal::g_testInfo.testName + "_" + artifactName;
}

// ----------------------------------------------------------------------------

unsigned
QuadsDrawingTest::FlushOnEvery()
{
  return Internal::g_testInfo.flushOnEvery;
}

// ----------------------------------------------------------------------------

const Chimia::Draw3D::MaterialID&
QuadsDrawingTest::ReferenceMaterial()
{
  return *Internal::g_referenceMaterial;
}

// ----------------------------------------------------------------------------

const Chimia::Draw3D::TextureID&
QuadsDrawingTest::ReferenceTexture()
{
  return *Internal::g_referenceTexture;
}

// ----------------------------------------------------------------------------