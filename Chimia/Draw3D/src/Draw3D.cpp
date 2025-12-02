#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "Shaders.h"
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
  LitWithVertexColorRendererImpl::getInstance().Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  VertexColoredRendererImpl::getInstance().Flush();
  LitRendererImpl::getInstance().Flush();
  LitWithVertexColorRendererImpl::getInstance().Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------