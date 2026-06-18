#include "QuadsDrawingFixture.h"

#include "Draw3D/Illumination.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"

#include "Media/Image.h"

#include "TestsUtils.h"

#include <memory>

// ----------------------------------------------------------------------------

namespace SampleData {
std::vector<glm::vec3> quad1Positions{
  { -0.5f, -0.5f, 0.0f },
  { 0.5f, -0.5f, 0.0f },
  { 0.5f, 0.5f, 0.0f },
  { -0.5f, 0.5f, 0.0f },

};

std::vector<glm::vec4> quad1Colors{ { 1.0f, 0.0f, 0.0f, 1.0f },
                                    { 0.0f, 1.0f, 0.0f, 1.0f },
                                    { 0.0f, 0.0f, 1.0f, 1.0f },
                                    { 1.0f, 0.0f, 1.0f, 1.0f } };

const float unitComponent = 0.577350269189626f;
std::vector<glm::vec3> quadNormals{
  { -unitComponent, -unitComponent, -unitComponent },
  { unitComponent, -unitComponent, -unitComponent },
  { unitComponent, unitComponent, -unitComponent },
  { -unitComponent, unitComponent, -unitComponent }
};

std::vector<glm::vec2> quadTexCoords{ { 0.0f, 0.0f },
                                      { 1.0f, 0.0f },
                                      { 1.0f, 1.0f },
                                      { 0.0f, 1.0f } };

std::vector<glm::vec3> quad2Positions{
  { -1.0f, -1.0f, 0.0f },
  { 0.0f, -1.0f, 0.0f },
  { 0.0f, 1.0f, 0.0f },
  { -1.0f, 1.0f, 0.0f },
};

std::vector<glm::vec4> quad2Colors{ { 1.0f, 0.0f, 0.0f, 1.0f },
                                    { 0.0f, 1.0f, 0.0f, 1.0f },
                                    { 0.0f, 0.0f, 1.0f, 1.0f },
                                    { 1.0f, 0.0f, 1.0f, 1.0f } };

std::vector<glm::vec3> quad3Positions{
  { 0.5f, -1.0f, 0.0f },
  { 1.0f, -1.0f, 0.0f },
  { 1.0f, 0.0f, 0.0f },
  { 0.5f, 0.0f, 0.0f },
};

std::vector<glm::vec4> quad3Colors{ { 1.0f, 0.0f, 1.0f, 1.0f },
                                    { 0.0f, 1.0f, 1.0f, 1.0f },
                                    { 0.0f, 0.0f, 1.0f, 1.0f },
                                    { 1.0f, 0.0f, 1.0f, 1.0f } };

std::vector<std::vector<glm::vec3>> quadPositions{ quad1Positions,
                                                   quad2Positions,
                                                   quad3Positions };

std::vector<std::vector<glm::vec4>> quadColors{ quad1Colors,
                                                quad2Colors,
                                                quad3Colors };
}

// ----------------------------------------------------------------------------

std::vector<Chimia::Draw3D::VertexPCNT>
QuadsDrawingFixture::QuadPCNT(size_t quadNumber)
{
  const size_t nQuads = NQuads();
  auto vertex = [quadNumber, nQuads](size_t i) {
    return Chimia::Draw3D::VertexPCNT{
      SampleData::quadPositions[quadNumber % nQuads][i],
      SampleData::quadColors[quadNumber % nQuads][i],
      SampleData::quadNormals[i],
      SampleData::quadTexCoords[i]
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

// ----------------------------------------------------------------------------

std::vector<Chimia::Draw3D::VertexPC>
QuadsDrawingFixture::QuadPC(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPC{
      quad[i].position,
      quad[i].color,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

// ----------------------------------------------------------------------------

std::vector<Chimia::Draw3D::VertexPN>
QuadsDrawingFixture::QuadPN(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPN{
      quad[i].position,
      quad[i].normal,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

// ----------------------------------------------------------------------------

std::vector<Chimia::Draw3D::VertexPT>
QuadsDrawingFixture::QuadPT(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPT{
      quad[i].position,
      quad[i].texCoord,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

// ----------------------------------------------------------------------------

std::vector<Chimia::Draw3D::VertexPCN>
QuadsDrawingFixture::QuadPCN(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPCN{
      quad[i].position,
      quad[i].color,
      quad[i].normal,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

// ----------------------------------------------------------------------------

std::vector<Chimia::Draw3D::VertexPCT>
QuadsDrawingFixture::QuadPCT(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPCT{
      quad[i].position,
      quad[i].color,
      quad[i].texCoord,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

// ----------------------------------------------------------------------------

size_t
QuadsDrawingFixture::NQuads()
{
  return std::max(SampleData::quadPositions.size(),
                  SampleData::quadColors.size());
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

namespace Internal {

std::unique_ptr<Chimia::Draw3D::MaterialID> g_referenceMaterial = nullptr;
std::unique_ptr<Chimia::Draw3D::TextureID> g_referenceTexture = nullptr;
TrianglesDrawingTestInfo g_testInfo;

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
QuadsDrawingFixture::Init(const TrianglesDrawingTestInfo& testInfo)
{
  Internal::g_testInfo = testInfo;
  if (Internal::g_testInfo.testName.empty()) {
    Internal::g_testInfo.testName = "testCommon";
  }

  Internal::Setup();
}

// ----------------------------------------------------------------------------

std::string
QuadsDrawingFixture::FullArtifactName(const std::string& artifactName)
{
  return Internal::g_testInfo.testName + "_" + artifactName;
}

// ----------------------------------------------------------------------------

unsigned
QuadsDrawingFixture::FlushOnEvery()
{
  return Internal::g_testInfo.flushOnEvery;
}

bool
QuadsDrawingFixture::ShouldVerifyRetainedRemovals()
{
  return Internal::g_testInfo.shouldVerifyRetainedRemovals;
}

// ----------------------------------------------------------------------------

const Chimia::Draw3D::MaterialID&
QuadsDrawingFixture::ReferenceMaterial()
{
  return *Internal::g_referenceMaterial;
}

// ----------------------------------------------------------------------------

const Chimia::Draw3D::TextureID&
QuadsDrawingFixture::ReferenceTexture()
{
  return *Internal::g_referenceTexture;
}

// ----------------------------------------------------------------------------