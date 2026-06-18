#include "TestTrianglesDrawing.h"

#include "Draw3D/Draw3D.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"

#include "Utils/Window.h"

#include "TestsUtils.h"

// ----------------------------------------------------------------------------
// Immediate mode
// Helper functions
// ----------------------------------------------------------------------------

template<typename QuadLayout>
void
DrawAllQuadsCommon(
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  unsigned flushOnEvery,
  const std::function<void(const QuadLayout& v1,
                           const QuadLayout& v2,
                           const QuadLayout& v3)>& drawTriangle)
{
  const size_t nQuads = QuadsDrawingFixture::NQuads();
  for (size_t i = 0; i < nQuads; ++i) {
    auto quad = getQuad(i);

    drawTriangle(quad[0], quad[1], quad[2]);
    drawTriangle(quad[2], quad[3], quad[0]);

    if ((i + 1) % flushOnEvery == 0) {
      Chimia::Draw3D::Flush();
    }
  }
}

// ----------------------------------------------------------------------------

template<typename QuadLayout>
void
DrawAllQuads(const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
             unsigned flushOnEvery = 1000)
{
  DrawAllQuadsCommon<QuadLayout>(
    getQuad,
    flushOnEvery,
    [](const QuadLayout& v1, const QuadLayout& v2, const QuadLayout& v3) {
      Chimia::Draw3D::Triangle(v1, v2, v3);
    });
}

// ----------------------------------------------------------------------------

template<typename QuadLayout, typename Resource>
void
DrawAllQuadsWithResource(
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  const Resource& resource,
  unsigned flushOnEvery = 1000)
{
  DrawAllQuadsCommon<QuadLayout>(
    getQuad,
    flushOnEvery,
    [resource](
      const QuadLayout& v1, const QuadLayout& v2, const QuadLayout& v3) {
      Chimia::Draw3D::Triangle(v1, v2, v3, resource);
    });
}

// ----------------------------------------------------------------------------
// Immediate mode
// Scenarios
// ----------------------------------------------------------------------------

namespace TriangleImmediateScenarios {

void
VertexColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuads<Chimia::Draw3D::VertexPC>(QuadsDrawingFixture::QuadPC,
                                         QuadsDrawingFixture::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingFixture::FullArtifactName("vertexColored.png"));
}

void
NormalMaterialColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPN>(
    QuadsDrawingFixture::QuadPN,
    QuadsDrawingFixture::ReferenceMaterial(),
    QuadsDrawingFixture::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingFixture::FullArtifactName("materialColoredLit.png"));
}

void
Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPT>(
    QuadsDrawingFixture::QuadPT,
    QuadsDrawingFixture::ReferenceTexture(),
    QuadsDrawingFixture::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingFixture::FullArtifactName("textured.png"));
}

void
VertexColored_Lit(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuads<Chimia::Draw3D::VertexPCN>(QuadsDrawingFixture::QuadPCN,
                                          QuadsDrawingFixture::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingFixture::FullArtifactName("vertexColoredLit.png"));
}

void
VertexColored_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCT>(
    QuadsDrawingFixture::QuadPCT,
    QuadsDrawingFixture::ReferenceTexture(),
    QuadsDrawingFixture::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingFixture::FullArtifactName("vertexColoredTextured.png"));
}

void
VertexColored_Lit_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCNT>(
    QuadsDrawingFixture::QuadPCNT,
    QuadsDrawingFixture::ReferenceTexture(),
    QuadsDrawingFixture::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingFixture::FullArtifactName("vertexColoredLitTextured.png"));
}

}

// ----------------------------------------------------------------------------
// Retained mode
// Helper functions
// ----------------------------------------------------------------------------

template<typename QuadLayout>
void
AddAndRemoveAllQuadsCommon(
  const std::string& mainArtifactName,
  Window& win,
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  unsigned flushOnEvery,
  std::function<Chimia::Draw3D::TriangleMeshID(const std::vector<QuadLayout>&)>
    addTriangle)
{
  std::vector<Chimia::Draw3D::TriangleMeshID> created;

  const size_t nQuads = QuadsDrawingFixture::NQuads();
  unsigned stepNumber = 0;

  for (size_t i = 0; i < nQuads; ++i) {
    Chimia::Draw3D::ClearScreen();

    auto quad = getQuad(i);

    const auto newRetainedQuad =
      addTriangle({ quad[0], quad[1], quad[2], quad[2], quad[3], quad[0] });
    created.push_back(newRetainedQuad);

    Chimia::Draw3D::Flush();
    win.Swap();

    const std::string imageName =
      mainArtifactName + std::to_string(++stepNumber) + ".png";
    TestsUtils::ExpectImage(QuadsDrawingFixture::FullArtifactName(imageName));
  }

  for (const auto& retainedQuad : created) {
    Chimia::Draw3D::ClearScreen();
    Chimia::Draw3D::DeleteRetainedTriangles(retainedQuad);

    Chimia::Draw3D::Flush();
    win.Swap();

    const std::string imageName =
      mainArtifactName + std::to_string(++stepNumber) + ".png";
    TestsUtils::ExpectImage(QuadsDrawingFixture::FullArtifactName(imageName));
  }
}

// ----------------------------------------------------------------------------

template<typename QuadLayout>
void
AddAndRemoveAllQuads(
  const std::string& mainArtifactName,
  Window& win,
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  unsigned flushOnEvery = 1000)
{
  AddAndRemoveAllQuadsCommon<QuadLayout>(
    mainArtifactName,
    win,
    getQuad,
    flushOnEvery,
    [](const std::vector<QuadLayout>& v) {
      return Chimia::Draw3D::AddRetainedTriangles(v);
    });
}

// ----------------------------------------------------------------------------

template<typename QuadLayout, typename Resource>
void
AddAndRemoveAllQuadsWithResource(
  const std::string& mainArtifactName,
  Window& win,
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  const Resource& resource,
  unsigned flushOnEvery = 1000)
{
  AddAndRemoveAllQuadsCommon<QuadLayout>(
    mainArtifactName,
    win,
    getQuad,
    flushOnEvery,
    [resource](const std::vector<QuadLayout>& v) {
      return Chimia::Draw3D::AddRetainedTriangles(v, resource);
    });
}

// ----------------------------------------------------------------------------
// Retained mode
// Scenarios
// ----------------------------------------------------------------------------

namespace TriangleRetainedScenarios {
void
VertexColored(Window& win)
{
  const std::string mainArtifactName = "Retained_vertexColored_step";

  AddAndRemoveAllQuads<Chimia::Draw3D::VertexPC>(
    mainArtifactName, win, QuadsDrawingFixture::QuadPC);
}

void
NormalWithMaterial(Window& win)
{
  const std::string mainArtifactName = "Retained_normalWithMaterial_step";

  AddAndRemoveAllQuadsWithResource<Chimia::Draw3D::VertexPN>(
    mainArtifactName,
    win,
    QuadsDrawingFixture::QuadPN,
    QuadsDrawingFixture::ReferenceMaterial());
}

void
Textured(Window& win)
{
  const std::string mainArtifactName = "Retained_textured_step";

  AddAndRemoveAllQuadsWithResource<Chimia::Draw3D::VertexPT>(
    mainArtifactName,
    win,
    QuadsDrawingFixture::QuadPT,
    QuadsDrawingFixture::ReferenceTexture());
}

void
VertexColoredAndLit(Window& win)
{
  const std::string mainArtifactName = "Retained_vertexColoredAndLit_step";

  AddAndRemoveAllQuads<Chimia::Draw3D::VertexPCN>(
    mainArtifactName, win, QuadsDrawingFixture::QuadPCN);
}

void
VertexColoredAndTextured(Window& win)
{
  const std::string mainArtifactName = "Retained_vertexColoredAndTextured_step";

  AddAndRemoveAllQuadsWithResource<Chimia::Draw3D::VertexPCT>(
    mainArtifactName,
    win,
    QuadsDrawingFixture::QuadPCT,
    QuadsDrawingFixture::ReferenceTexture());
}

void
VertexColoredLitAndTextured(Window& win)
{
  const std::string mainArtifactName =
    "Retained_vertexColoredLitAndTextured_step";

  AddAndRemoveAllQuadsWithResource<Chimia::Draw3D::VertexPCNT>(
    mainArtifactName,
    win,
    QuadsDrawingFixture::QuadPCNT,
    QuadsDrawingFixture::ReferenceTexture());
}
}

// ----------------------------------------------------------------------------

void
TestImmediateModeTriangles(const TrianglesDrawingTestInfo& testInfo,
                           Window& window)
{
  QuadsDrawingFixture::Init(testInfo);

  TriangleImmediateScenarios::VertexColored(window);
  TriangleImmediateScenarios::NormalMaterialColored(window);
  TriangleImmediateScenarios::Textured(window);
  TriangleImmediateScenarios::VertexColored_Lit(window);
  TriangleImmediateScenarios::VertexColored_Textured(window);
  TriangleImmediateScenarios::VertexColored_Lit_Textured(window);
}

// ----------------------------------------------------------------------------

void
TestRetainedModeTriangles(const TrianglesDrawingTestInfo& testInfo,
                          Window& window)
{
  QuadsDrawingFixture::Init(testInfo);

  TriangleRetainedScenarios::VertexColored(window);
  TriangleRetainedScenarios::NormalWithMaterial(window);
  TriangleRetainedScenarios::Textured(window);
  TriangleRetainedScenarios::VertexColoredAndLit(window);
  TriangleRetainedScenarios::VertexColoredAndTextured(window);
  TriangleRetainedScenarios::VertexColoredLitAndTextured(window);
}

// ----------------------------------------------------------------------------