#include "TestImmediateModeTriangles.h"

#include "Draw3D/Draw3D.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"

#include "Utils/Window.h"

#include "GraphicsTestsData.h"
#include "QuadsDrawingTest.h"

#include "TestsUtils.h"

// ----------------------------------------------------------------------------

template<typename QuadLayout>
void
DrawAllQuads(const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
             unsigned flushOnEvery = 1000)
{
  const size_t nQuads = GraphicsTestsData::NQuads();
  for (size_t i = 0; i < nQuads; ++i) {
    auto quad = getQuad(i);

    Chimia::Draw3D::Triangle(quad[0], quad[1], quad[2]);
    Chimia::Draw3D::Triangle(quad[2], quad[3], quad[0]);

    if ((i + 1) % flushOnEvery == 0) {
      Chimia::Draw3D::Flush();
    }
  }
}

template<typename QuadLayout, typename Resource>
void
DrawAllQuadsWithResource(
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  const Resource& resource,
  unsigned flushOnEvery = 1000)
{
  const size_t nQuads = GraphicsTestsData::NQuads();
  for (size_t i = 0; i < nQuads; ++i) {
    auto quad = getQuad(i);

    Chimia::Draw3D::Triangle(quad[0], quad[1], quad[2], resource);
    Chimia::Draw3D::Triangle(quad[2], quad[3], quad[0], resource);

    if ((i + 1) % flushOnEvery == 0) {
      Chimia::Draw3D::Flush();
    }
  }
}

// ----------------------------------------------------------------------------

namespace Scenarios {

void
VertexColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuads<Chimia::Draw3D::VertexPC>(GraphicsTestsData::QuadPC,
                                         QuadsDrawingTest::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingTest::FullArtifactName("vertexColored.png"));
}

void
NormalMaterialColored(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPN>(
    GraphicsTestsData::QuadPN,
    QuadsDrawingTest::ReferenceMaterial(),
    QuadsDrawingTest::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingTest::FullArtifactName("materialColoredLit.png"));
}

void
Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPT>(
    GraphicsTestsData::QuadPT,
    QuadsDrawingTest::ReferenceTexture(),
    QuadsDrawingTest::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(QuadsDrawingTest::FullArtifactName("textured.png"));
}

void
VertexColored_Lit(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuads<Chimia::Draw3D::VertexPCN>(GraphicsTestsData::QuadPCN,
                                          QuadsDrawingTest::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingTest::FullArtifactName("vertexColoredLit.png"));
}

void
VertexColored_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCT>(
    GraphicsTestsData::QuadPCT,
    QuadsDrawingTest::ReferenceTexture(),
    QuadsDrawingTest::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingTest::FullArtifactName("vertexColoredTextured.png"));
}

void
VertexColored_Lit_Textured(Window& win)
{
  Chimia::Draw3D::ClearScreen();

  DrawAllQuadsWithResource<Chimia::Draw3D::VertexPCNT>(
    GraphicsTestsData::QuadPCNT,
    QuadsDrawingTest::ReferenceTexture(),
    QuadsDrawingTest::FlushOnEvery());

  Chimia::Draw3D::Flush();

  win.Swap();

  TestsUtils::ExpectImage(
    QuadsDrawingTest::FullArtifactName("vertexColoredLitTextured.png"));
}

}

// ----------------------------------------------------------------------------

void
TestImmediateModeTriangles(const ImmediateTrianglesTestInfo& testInfo,
                           Window& window)
{
  QuadsDrawingTest::Init(testInfo);

  Scenarios::VertexColored(window);
  Scenarios::NormalMaterialColored(window);
  Scenarios::Textured(window);
  Scenarios::VertexColored_Lit(window);
  Scenarios::VertexColored_Textured(window);
  Scenarios::VertexColored_Lit_Textured(window);
}

// ----------------------------------------------------------------------------