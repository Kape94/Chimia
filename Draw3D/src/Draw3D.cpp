#include "Draw3D.h"

// ----------------------------------------------------------------------------

#include "TriangleDetail.h"

#include "Rendering/Rendering.h"

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Initialize()
{
  Chimia::Rendering::Initialize();

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