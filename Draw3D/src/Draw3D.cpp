#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "Shaders.h"
#include "TrianglePrivate.h"

#include "Rendering/Rendering.h"

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();

  Shaders::Initialize();
  TrianglePrivate::Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  TrianglePrivate::Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------