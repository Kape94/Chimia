#include "Renderers.h"

#include "LitRendererImpl.h"
#include "VertexColoredRendererImpl.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

VertexColoredRenderer&
Chimia::Draw3D::GetVertexColoredRenderer()
{
  return VertexColoredRendererImpl::getInstance();
}

// ----------------------------------------------------------------------------

LitRenderer&
Chimia::Draw3D::GetLitRenderer()
{
  return LitRendererImpl::getInstance();
}

// ----------------------------------------------------------------------------