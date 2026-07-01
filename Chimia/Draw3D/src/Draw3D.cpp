#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingPrivate.h"
#include "Pipelines.h"
#include "Shaders.h"
#include "TrianglePrivate.h"

#include "defaultRenderers/Color4.h"
#include "defaultRenderers/Color4Normal3.h"
#include "defaultRenderers/Color4Normal3TexCoord2.h"
#include "defaultRenderers/Color4TexCoord2.h"
#include "defaultRenderers/Normal3.h"
#include "defaultRenderers/Normal3TexCoord2.h"
#include "defaultRenderers/TexCoord2.h"

#include "Rendering/Rendering.h"
#include "eImmediateFlushingPolicy.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
void
InitRenderers()
{
  DefaultRenderers::Color4::Init();
  DefaultRenderers::Normal3::Init();
  DefaultRenderers::TexCoord2::Init();
  DefaultRenderers::Color4TexCoord2::Init();
  DefaultRenderers::Color4Normal3::Init();
  DefaultRenderers::Normal3TexCoord2::Init();
  DefaultRenderers::Color4Normal3TexCoord2::Init();

  TrianglePrivate::Init();
  ModelRenderingPrivate::Init();
}

void
RestartRenderers()
{
  DefaultRenderers::Color4::Shutdown();
  DefaultRenderers::Normal3::Shutdown();
  DefaultRenderers::TexCoord2::Shutdown();
  DefaultRenderers::Color4TexCoord2::Shutdown();
  DefaultRenderers::Color4Normal3::Shutdown();
  DefaultRenderers::Normal3TexCoord2::Shutdown();
  DefaultRenderers::Color4Normal3TexCoord2::Shutdown();

  InitRenderers();
}

void
FlushRenderers(const eImmediateFlusingPolicy flushingPolicy)
{
  DefaultRenderers::Color4::GetRenderer().Flush(flushingPolicy);
  DefaultRenderers::Normal3::GetRenderer().Flush(flushingPolicy);
  DefaultRenderers::TexCoord2::GetRenderer().Flush(flushingPolicy);
  DefaultRenderers::Color4Normal3::GetRenderer().Flush(flushingPolicy);
  DefaultRenderers::Color4TexCoord2::GetRenderer().Flush(flushingPolicy);
  DefaultRenderers::Normal3TexCoord2::GetRenderer().Flush(flushingPolicy);
  DefaultRenderers::Color4Normal3TexCoord2::GetRenderer().Flush(flushingPolicy);
}
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();
  Chimia::Rendering::EnableDepthTest(true);

  Shaders::Initialize();
  Pipelines::Init();

  InitRenderers();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  FlushRenderers(eImmediateFlusingPolicy::RENDER_AND_KEEP_INPUTS);

  Pipelines::ActivateTransparentRenderingPipeline();

  FlushRenderers(eImmediateFlusingPolicy::RENDER_AND_FLUSH_INPUTS);

  Pipelines::ActivateRegularPipeline();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Chimia::Rendering::Clear(0.0f, 0.0f, 0.0f, 1.0f);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Restart()
{
  RestartRenderers();
}

// ----------------------------------------------------------------------------