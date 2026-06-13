#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingPrivate.h"
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
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();
  Chimia::Rendering::EnableDepthTest(true);

  Shaders::Initialize();

  InitRenderers();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  DefaultRenderers::Color4::GetRenderer().Flush();
  DefaultRenderers::Normal3::GetRenderer().Flush();
  DefaultRenderers::TexCoord2::GetRenderer().Flush();
  DefaultRenderers::Color4Normal3::GetRenderer().Flush();
  DefaultRenderers::Color4TexCoord2::GetRenderer().Flush();
  DefaultRenderers::Normal3TexCoord2::GetRenderer().Flush();
  DefaultRenderers::Color4Normal3TexCoord2::GetRenderer().Flush();

  Chimia::Rendering::EnableDepthMask(false);
  Chimia::Rendering::EnableColorBlend(true);

  DefaultRenderers::Color4::GetTransparentRenderer().Flush();
  DefaultRenderers::TexCoord2::GetTransparentRenderer().Flush();
  DefaultRenderers::Color4TexCoord2::GetTransparentRenderer().Flush();

  Chimia::Rendering::EnableDepthMask(true);
  Chimia::Rendering::EnableColorBlend(false);
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