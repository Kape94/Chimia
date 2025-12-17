#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

struct VertexLayoutAttributes
{
  Chimia::Rendering::ShaderAttributes vertexAttributes;
  Chimia::Rendering::ShaderAttributes instancedAttributes;
};

VertexLayoutAttributes
GetAttributesForLayout(const eVertexLayout& layout);

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------