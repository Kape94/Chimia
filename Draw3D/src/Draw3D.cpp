#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "LitTrianglePrivate.h"
#include "Shaders.h"
#include "TrianglePrivate.h"

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
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  TrianglePrivate::Flush();
  LitTrianglePrivate::Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------