#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "ColoredTexturedLitRendererImpl.h"
#include "ColoredTexturedRendererImpl.h"
#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "Shaders.h"
#include "TexturedLitRendererImpl.h"
#include "TexturedRendererImpl.h"
#include "VertexColoredRendererImpl.h"

#include "Rendering/Rendering.h"

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();
  Chimia::Rendering::EnableDepthTest(true);

  Shaders::Initialize();

  VertexColoredRendererImpl::GetRenderer().Init();
  LitRendererImpl::GetRenderer().Init();
  TexturedRendererImpl::GetRenderer().Init();
  ColoredTexturedRendererImpl::GetRenderer().Init();
  LitWithVertexColorRendererImpl::GetRenderer().Init();
  TexturedLitRendererImpl::GetRenderer().Init();
  ColoredTexturedLitRendererImpl::GetRenderer().Init();
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