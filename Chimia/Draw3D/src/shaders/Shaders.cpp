#include "Shaders.h"
// ----------------------------------------------------------------------------

#include "ShaderCodebase.h"

#include "glslCodes/Common.h"
#include "glslCodes/FragmentStage.h"
#include "glslCodes/VertexStage.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
Chimia::Rendering::Shader generic;

void
InitializeCodebase()
{
  ShaderCodebase::AddCodes(
    { // ----------------------- Common --------------------------

      { "common::constants", ShaderCodes::Common::constants },
      { "common::lightsTypes", ShaderCodes::Common::lightsTypes },
      { "common::lightsUniforms", ShaderCodes::Common::lightsUniforms },
      { "common::materialType", ShaderCodes::Common::materialType },
      { "common::calculateLights", ShaderCodes::Common::calculateLights },
      { "common::calculateLightsWithoutMaterial",
        ShaderCodes::Common::calculateLightsWithoutMaterial },

      // ----------------------- Vertex --------------------------

      { "vertex::generic", ShaderCodes::Vertex::generic },

      // ----------------------- Fragment --------------------------

      { "fragment::generic", ShaderCodes::Fragment::generic } });
}

}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Shaders::Initialize()
{
  InitializeCodebase();

  generic.Create(ShaderCodebase::Code("vertex::generic"),
                 ShaderCodebase::Code("fragment::generic"));
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::Generic()
{
  return generic;
}

// ----------------------------------------------------------------------------