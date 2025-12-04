#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "Shaders.h"
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
  VertexColoredRendererImpl::getInstance().Init();
  LitRendererImpl::getInstance().Init();
  TexturedRendererImpl::getInstance().Init();
  LitWithVertexColorRendererImpl::getInstance().Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  VertexColoredRendererImpl::getInstance().Flush();
  LitRendererImpl::getInstance().Flush();
  TexturedRendererImpl::getInstance().Flush();
  LitWithVertexColorRendererImpl::getInstance().Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------