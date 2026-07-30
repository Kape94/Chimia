#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "GenericRenderer.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace Renderers {
GenericRenderer&
CreateRenderer(const eVertexLayout& vertexLayout,
               const Rendering::Shader& shader,
               void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
               void (*setupShaderForInstancedRendering)(const ResourcesGroup&));

void
DeleteRenderer(GenericRenderer* renderer);

GenericRenderer*
GetRendererByID(const unsigned id);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------