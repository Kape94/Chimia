#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "ColoredTexturedLitRendererImpl.h"
#include "ColoredTexturedRendererImpl.h"
#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "ModelRenderingPrivate.h"
#include "Shaders.h"
#include "TexturedLitRendererImpl.h"
#include "TexturedRendererImpl.h"
#include "TrianglePrivate.h"
#include "VertexColoredRendererImpl.h"

#include "Rendering/Rendering.h"

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();
  Chimia::Rendering::EnableDepthTest(true);

  Shaders::Initialize();

  VertexColoredRendererImpl::Init();
  LitRendererImpl::Init();
  TexturedRendererImpl::Init();
  ColoredTexturedRendererImpl::Init();
  LitWithVertexColorRendererImpl::Init();
  TexturedLitRendererImpl::Init();
  ColoredTexturedLitRendererImpl::Init();

  TrianglePrivate::Init();
  ModelRenderingPrivate::Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  VertexColoredRendererImpl::GetRenderer().Flush();
  LitRendererImpl::GetRenderer().Flush();
  TexturedRendererImpl::GetRenderer().Flush();
  ColoredTexturedRendererImpl::GetRenderer().Flush();
  LitWithVertexColorRendererImpl::GetRenderer().Flush();
  TexturedLitRendererImpl::GetRenderer().Flush();
  ColoredTexturedLitRendererImpl::GetRenderer().Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------