#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "Shaders.h"
#include "TriangleDetail.h"

#include "Rendering/Rendering.h"

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();

  Shaders::Initialize();
  TriangleDetail::Init();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Flush()
{
  TriangleDetail::Flush();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::ClearScreen()
{
  Rendering::Clear();
}

// ----------------------------------------------------------------------------