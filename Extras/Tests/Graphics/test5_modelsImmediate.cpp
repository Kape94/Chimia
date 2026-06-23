#include "CommonTestingFixture.h"
#include "Draw3D/Draw3D.h"

#include "Draw3D/ModelRendering.h"
#include "Draw3D/Types.h"
#include "Utils/Window.h"

#include "ModelsDrawingFixture.h"

#include "TestsUtils.h"

namespace Scenarios {

void
DrawModelsCommon(
  const Chimia::Draw3D::eVertexLayout& layout,
  std::function<void(const Chimia::Draw3D::ModelID&, const glm::mat4x4&)> draw,
  Window& win)
{
  const auto model = ModelsDrawingFixture::GetModel(layout);

  Chimia::Draw3D::ClearScreen();

  for (const auto& t : ModelsDrawingFixture::GetTransforms()) {
    draw(model, t);
  }

  Chimia::Draw3D::Flush();
  win.Swap();
}

void
DrawModels(const Chimia::Draw3D::eVertexLayout& layout, Window& win)
{
  DrawModelsCommon(
    layout,
    [](const Chimia::Draw3D::ModelID& model, const glm::mat4x4& transform) {
      Chimia::Draw3D::DrawModel(model, transform);
    },
    win);
}

template<typename Resource>
void
DrawModelsWithResource(const Chimia::Draw3D::eVertexLayout& layout,
                       const Resource& resource,
                       Window& win)
{
  DrawModelsCommon(
    layout,
    [resource](const Chimia::Draw3D::ModelID& model,
               const glm::mat4x4& transform) {
      Chimia::Draw3D::DrawModel(model, transform, resource);
    },
    win);
}

void
ImmediateDraw(Window& win)
{
  DrawModels(Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4, win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelVertexColored.png"));

  DrawModelsWithResource(Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3,
                         CommonTestingFixture::ReferenceMaterial(),
                         win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelLitMaterialColored.png"));

  DrawModelsWithResource(Chimia::Draw3D::eVertexLayout::POSITION3_TEXCOORD2,
                         CommonTestingFixture::ReferenceTexture(),
                         win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelTextured.png"));

  DrawModels(Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3, win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelVertexColoredLit.png"));

  DrawModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_TEXCOORD2,
    CommonTestingFixture::ReferenceTexture(),
    win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelVertexColoredTextured.png"));

  DrawModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2,
    CommonTestingFixture::ReferenceTexture(),
    win);

  TestsUtils::ExpectImage(CommonTestingFixture::FullArtifactName(
    "modelVertexColoredTexturedLit.png"));
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #5");

  TestsUtils::InitTesting(argv, win);

  Chimia::Draw3D::Initialize();

  CommonTestingConfig testInfo;
  testInfo.testName = "test5";
  testInfo.flushOnEvery = 1000;
  ModelsDrawingFixture::Init(testInfo);

  Scenarios::ImmediateDraw(win);

  return 0;
}
