#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "ModelRenderingPrivate.h"
#include "Shaders.h"
#include "TrianglePrivate.h"

#include "defaultRenderers/Color3.h"
#include "defaultRenderers/Color3Normal3.h"
#include "defaultRenderers/Color3Normal3TexCoord2.h"
#include "defaultRenderers/Color3TexCoord2.h"
#include "defaultRenderers/Normal3.h"
#include "defaultRenderers/Normal3TexCoord2.h"
#include "defaultRenderers/TexCoord2.h"

#include "Rendering/Rendering.h"

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();
  Chimia::Rendering::EnableDepthTest(true);

  Shaders::Initialize();

  DefaultRenderers::Color3::Init();
  DefaultRenderers::Normal3::Init();
  DefaultRenderers::TexCoord2::Init();
  DefaultRenderers::Color3TexCoord2::Init();
  DefaultRenderers::Color3Normal3::Init();
  DefaultRenderers::Normal3TexCoord2::Init();
  DefaultRenderers::Color3Normal3TexCoord2::Init();

  TrianglePrivate::Init();
  ModelRenderingPrivate::Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  DefaultRenderers::Color3::GetRenderer().Flush();
  DefaultRenderers::Normal3::GetRenderer().Flush();
  DefaultRenderers::TexCoord2::GetRenderer().Flush();
  DefaultRenderers::Color3Normal3::GetRenderer().Flush();
  DefaultRenderers::Color3TexCoord2::GetRenderer().Flush();
  DefaultRenderers::Normal3TexCoord2::GetRenderer().Flush();
  DefaultRenderers::Color3Normal3TexCoord2::GetRenderer().Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------