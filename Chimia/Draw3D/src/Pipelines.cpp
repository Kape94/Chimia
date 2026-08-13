#include "Pipelines.h"
#include "Pipeline.h"

#include "DataNames.h"

#include "Rendering/Rendering.h"
#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace PipelinesInternal {
std::unique_ptr<Pipeline> g_regularPipeline;
std::unique_ptr<Pipeline> g_transparentRenderingPipeline;

Pipeline* g_currentPipeline = nullptr;

}

// ----------------------------------------------------------------------------

void
Pipelines::Init()
{
  using namespace PipelinesInternal;

  g_regularPipeline.reset(new Pipeline(
    []() {
      Chimia::Rendering::EnableDepthMask(true);
      Chimia::Rendering::EnableColorBlend(false);
    },
    [](Chimia::Rendering::ShaderInstance& shader) {
      shader->SetUniform(DataNames::ShaderUniforms::IS_TRANSPARENT_RENDERING,
                         false);
    }));

  g_transparentRenderingPipeline.reset(new Pipeline(
    []() {
      Chimia::Rendering::EnableDepthMask(false);
      Chimia::Rendering::EnableColorBlend(true);
    },
    [](Chimia::Rendering::ShaderInstance& shader) {
      shader->SetUniform(DataNames::ShaderUniforms::IS_TRANSPARENT_RENDERING,
                         true);
    }));

  g_currentPipeline = g_regularPipeline.get();

  g_currentPipeline->Use();
}

// ----------------------------------------------------------------------------

void
Pipelines::ActivateRegularPipeline()
{
  PipelinesInternal::g_regularPipeline->Use();
  PipelinesInternal::g_currentPipeline =
    PipelinesInternal::g_regularPipeline.get();
}

// ----------------------------------------------------------------------------

void
Pipelines::ActivateTransparentRenderingPipeline()
{
  PipelinesInternal::g_transparentRenderingPipeline->Use();
  PipelinesInternal::g_currentPipeline =
    PipelinesInternal::g_transparentRenderingPipeline.get();
}

// ----------------------------------------------------------------------------

const Pipeline&
Pipelines::CurrentPipeline()
{
  return *PipelinesInternal::g_currentPipeline;
}

// ----------------------------------------------------------------------------