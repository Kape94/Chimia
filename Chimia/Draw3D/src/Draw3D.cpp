#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "LitRendererImpl.h"
#include "LitTrianglePrivate.h"
#include "Shaders.h"
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
  TrianglePrivate::Init();
  LitTrianglePrivate::Init();
  VertexColoredRendererImpl::getInstance().Init();
  LitRendererImpl::getInstance().Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  TrianglePrivate::Flush();
  LitTrianglePrivate::Flush();
  VertexColoredRendererImpl::getInstance().Flush();
  LitRendererImpl::getInstance().Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------