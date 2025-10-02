#include "Shaders.h"
// ----------------------------------------------------------------------------

#include "ShaderCodes.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
Chimia::Rendering::Shader vertexColored;
Chimia::Rendering::Shader gouraudLit;
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Shaders::Initialize()
{
  vertexColored.Create(ShaderCodes::Vertex::colored,
                       ShaderCodes::Fragment::colored);

  gouraudLit.Create(ShaderCodes::Vertex::gouraudLit,
                    ShaderCodes::Fragment::gouraudLit);
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::VertexColored()
{
  return vertexColored;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLit()
{
  return gouraudLit;
}

// ----------------------------------------------------------------------------