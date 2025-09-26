#include "Shaders.h"
// ----------------------------------------------------------------------------

#include "ShaderCodes.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
Chimia::Rendering::Shader vertexColored;
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Shaders::Initialize()
{
  vertexColored.Create(ShaderCodes::Vertex::vertexColored,
                       ShaderCodes::Fragment::colored);
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::VertexColored()
{
  return vertexColored;
}

// ----------------------------------------------------------------------------