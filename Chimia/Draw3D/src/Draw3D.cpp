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
  LitRendererImpl::getInstance().Init();
  TexturedRendererImpl::GetRenderer().Init();
  ColoredTexturedRendererImpl::getInstance().Init();
  LitWithVertexColorRendererImpl::getInstance().Init();
  TexturedLitRendererImpl::getInstance().Init();
  ColoredTexturedLitRendererImpl::getInstance().Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  VertexColoredRendererImpl::GetRenderer().Flush();
  LitRendererImpl::getInstance().Flush();
  TexturedRendererImpl::GetRenderer().Flush();
  ColoredTexturedRendererImpl::getInstance().Flush();
  LitWithVertexColorRendererImpl::getInstance().Flush();
  TexturedLitRendererImpl::getInstance().Flush();
  ColoredTexturedLitRendererImpl::getInstance().Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------