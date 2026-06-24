#include "TestModelsDrawing.h"

#include "CommonTestingFixture.h"
#include "Draw3D/Draw3D.h"
#include "Draw3D/ModelRendering.h"
#include "Draw3D/Types.h"

#include "ModelsDrawingFixture.h"
#include "Utils/Window.h"

#include "TestsUtils.h"

// ----------------------------------------------------------------------------

bool
ShouldFlushAndVerify(const size_t i,
                     const size_t nItems,
                     const unsigned flushOnEvery)
{
  const size_t base1Index = i + 1;

  const bool noIntermediateFlushing =
    flushOnEvery > nItems && base1Index == nItems;

  const bool shouldFlushOnIndex = base1Index % flushOnEvery == 0;

  return shouldFlushOnIndex || noIntermediateFlushing;
}

// ----------------------------------------------------------------------------
// Immediate mode
// Helper functions
// ----------------------------------------------------------------------------

void
DrawModelsCommon(
  const Chimia::Draw3D::eVertexLayout& layout,
  std::function<void(const Chimia::Draw3D::ModelID&, const glm::mat4x4&)> draw,
  const unsigned flushOnEvery,
  Window& win)
{
  const auto model = ModelsDrawingFixture::GetModel(layout);

  Chimia::Draw3D::ClearScreen();

  const auto& transforms = ModelsDrawingFixture::GetTransforms();
  for (size_t i = 0; i < transforms.size(); ++i) {
    const auto& t = transforms[i];
    draw(model, t);

    if (ShouldFlushAndVerify(i, transforms.size(), 1000)) {
      Chimia::Draw3D::Flush();
    }
  }

  win.Swap();
}

void
DrawModels(const Chimia::Draw3D::eVertexLayout& layout,
           const unsigned flushOnEvery,
           Window& win)
{
  DrawModelsCommon(
    layout,
    [](const Chimia::Draw3D::ModelID& model, const glm::mat4x4& transform) {
      Chimia::Draw3D::DrawModel(model, transform);
    },
    flushOnEvery,
    win);
}

template<typename Resource>
void
DrawModelsWithResource(const Chimia::Draw3D::eVertexLayout& layout,
                       const Resource& resource,
                       const unsigned flushOnEvery,
                       Window& win)
{
  DrawModelsCommon(
    layout,
    [resource](const Chimia::Draw3D::ModelID& model,
               const glm::mat4x4& transform) {
      Chimia::Draw3D::DrawModel(model, transform, resource);
    },
    flushOnEvery,
    win);
}

// ----------------------------------------------------------------------------
// Immediate mode
// Scenarios
// ----------------------------------------------------------------------------

namespace TestImmediateMode {

void
DrawAllModels(Window& win)
{
  const unsigned flushOnEvery = CommonTestingFixture::FlushOnEvery();

  DrawModels(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4, flushOnEvery, win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelVertexColored.png"));

  DrawModelsWithResource(Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3,
                         CommonTestingFixture::ReferenceMaterial(),
                         flushOnEvery,
                         win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelLitMaterialColored.png"));

  DrawModelsWithResource(Chimia::Draw3D::eVertexLayout::POSITION3_TEXCOORD2,
                         CommonTestingFixture::ReferenceTexture(),
                         flushOnEvery,
                         win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelTextured.png"));

  DrawModels(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3, flushOnEvery, win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelVertexColoredLit.png"));

  DrawModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_TEXCOORD2,
    CommonTestingFixture::ReferenceTexture(),
    flushOnEvery,
    win);

  TestsUtils::ExpectImage(
    CommonTestingFixture::FullArtifactName("modelVertexColoredTextured.png"));

  DrawModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2,
    CommonTestingFixture::ReferenceTexture(),
    flushOnEvery,
    win);

  TestsUtils::ExpectImage(CommonTestingFixture::FullArtifactName(
    "modelVertexColoredTexturedLit.png"));
}

}

// ----------------------------------------------------------------------------
// Retained mode
// Helper functions
// ----------------------------------------------------------------------------

std::string
CurrentImageName(const std::string& baseImageName, const unsigned step)
{
  return baseImageName + "_" + std::to_string(step) + ".png";
}

std::vector<Chimia::Draw3D::ModelInstanceID>
AddModelsCommon(Chimia::Draw3D::eVertexLayout layout,
                const std::string& outputImageName,
                const unsigned flushOnEvery,
                const std::function<Chimia::Draw3D::ModelInstanceID(
                  const Chimia::Draw3D::ModelID&,
                  const glm::mat4x4&)> addModel,
                Window& win,
                unsigned& currentStep)
{
  auto model = ModelsDrawingFixture::GetModel(layout);

  std::vector<Chimia::Draw3D::ModelInstanceID> instances;

  const auto& transforms = ModelsDrawingFixture::GetTransforms();
  for (size_t i = 0; i < transforms.size(); ++i) {
    Chimia::Draw3D::ClearScreen();

    const auto& t = transforms[i];
    const auto newInstance = addModel(model, t);
    instances.push_back(newInstance);

    if (ShouldFlushAndVerify(i, transforms.size(), flushOnEvery)) {
      Chimia::Draw3D::Flush();
      win.Swap();

      const std::string imageName =
        CurrentImageName(outputImageName, currentStep++);

      TestsUtils::ExpectImage(imageName);
    }
  }

  return instances;
}

void
RemoveModelsCommon(
  const std::vector<Chimia::Draw3D::ModelInstanceID>& instances,
  const std::string& outputImageName,
  const bool shouldVerifyRetainedRemovals,
  Window& win,
  unsigned& currentStep)
{
  for (const auto& addedModel : instances) {
    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::DeleteRetainedModel(addedModel);

    Chimia::Draw3D::Flush();
    win.Swap();

    if (shouldVerifyRetainedRemovals) {
      const std::string imageName =
        CurrentImageName(outputImageName, currentStep++);

      TestsUtils::ExpectImage(imageName);
    }
  }
}

void
AddAndRemoveModelsCommon(Chimia::Draw3D::eVertexLayout layout,
                         const std::string& outputImageName,
                         const unsigned flushOnEvery,
                         const bool shouldVerifyRetainedRemovals,
                         const std::function<Chimia::Draw3D::ModelInstanceID(
                           const Chimia::Draw3D::ModelID&,
                           const glm::mat4x4&)> addModel,
                         Window& win)
{
  unsigned currentStep = 1;

  const std::vector<Chimia::Draw3D::ModelInstanceID> instances =
    AddModelsCommon(
      layout, outputImageName, flushOnEvery, addModel, win, currentStep);

  RemoveModelsCommon(
    instances, outputImageName, shouldVerifyRetainedRemovals, win, currentStep);
}

void
AddAndRemoveModels(Chimia::Draw3D::eVertexLayout layout,
                   const std::string& outputImageName,
                   const unsigned flushOnEvery,
                   const bool shouldVerifyRetainedRemovals,
                   Window& win)
{
  AddAndRemoveModelsCommon(
    layout,
    outputImageName,
    flushOnEvery,
    shouldVerifyRetainedRemovals,
    [](const Chimia::Draw3D::ModelID& model, const glm::mat4x4& transform) {
      return Chimia::Draw3D::AddRetainedModel(model, transform);
    },
    win);
}

template<typename Resource>
void
AddAndRemoveModelsWithResource(Chimia::Draw3D::eVertexLayout layout,
                               const Resource& resource,
                               const std::string& outputImageName,
                               const unsigned flushOnEvery,
                               const bool shouldVerifyRetainedRemovals,
                               Window& win)
{
  AddAndRemoveModelsCommon(
    layout,
    outputImageName,
    flushOnEvery,
    shouldVerifyRetainedRemovals,
    [resource](const Chimia::Draw3D::ModelID& model,
               const glm::mat4x4& transform) {
      return Chimia::Draw3D::AddRetainedModel(model, transform, resource);
    },
    win);
}

// ----------------------------------------------------------------------------
// Retained mode
// Scenarios
// ----------------------------------------------------------------------------

namespace TestRetainedMode {

void
AddAndRemoveAllModels(Window& win)
{
  // PC
  AddAndRemoveModels(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4,
    CommonTestingFixture::FullArtifactName("RetainedVertexColored_step"),
    CommonTestingFixture::FlushOnEvery(),
    CommonTestingFixture::ShouldVerifyRetainedRemovals(),
    win);

  // PN
  AddAndRemoveModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3,
    CommonTestingFixture::ReferenceMaterial(),
    CommonTestingFixture::FullArtifactName("RetainedLitMaterialColored_step"),
    CommonTestingFixture::FlushOnEvery(),
    CommonTestingFixture::ShouldVerifyRetainedRemovals(),
    win);

  // PT
  AddAndRemoveModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_TEXCOORD2,
    CommonTestingFixture::ReferenceTexture(),
    CommonTestingFixture::FullArtifactName("RetainedTextured_step"),
    CommonTestingFixture::FlushOnEvery(),
    CommonTestingFixture::ShouldVerifyRetainedRemovals(),
    win);

  // PCN
  AddAndRemoveModels(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3,
    CommonTestingFixture::FullArtifactName("RetainedVertexColoredLit_step"),
    CommonTestingFixture::FlushOnEvery(),
    CommonTestingFixture::ShouldVerifyRetainedRemovals(),
    win);

  // PCT
  AddAndRemoveModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_TEXCOORD2,
    CommonTestingFixture::ReferenceTexture(),
    CommonTestingFixture::FullArtifactName(
      "RetainedVertexColoredTextured_step"),
    CommonTestingFixture::FlushOnEvery(),
    CommonTestingFixture::ShouldVerifyRetainedRemovals(),
    win);

  // PCNT
  AddAndRemoveModelsWithResource(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2,
    CommonTestingFixture::ReferenceTexture(),
    CommonTestingFixture::FullArtifactName(
      "RetainedVertexColoredLitTextured_step"),
    CommonTestingFixture::FlushOnEvery(),
    CommonTestingFixture::ShouldVerifyRetainedRemovals(),
    win);
}

}

// ----------------------------------------------------------------------------
// TestModelsDrawing
// ----------------------------------------------------------------------------

void
TestImmediateModeModels(const CommonTestingConfig& testInfo, Window& window)
{
  ModelsDrawingFixture::Init(testInfo);

  TestImmediateMode::DrawAllModels(window);
}

// ----------------------------------------------------------------------------

void
TestRetainedModeModels(const CommonTestingConfig& testInfo, Window& window)
{
  ModelsDrawingFixture::Init(testInfo);

  TestRetainedMode::AddAndRemoveAllModels(window);
}

// ----------------------------------------------------------------------------