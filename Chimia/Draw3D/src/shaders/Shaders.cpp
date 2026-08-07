#include "Shaders.h"
// ----------------------------------------------------------------------------

#include "Rendering/DataLayout.h"
#include "ShaderCodebase.h"

#include "glslCodes/Common.h"
#include "glslCodes/FragmentStage.h"
#include "glslCodes/VertexStage.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
Chimia::Rendering::ShaderInstance generic;

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

  generic = Rendering::Shader::Create(
    ShaderCodebase::Code("vertex::generic"),
    ShaderCodebase::Code("fragment::generic"),
    { { "a_vertexPos", Rendering::eDataType::VECTOR_3_FLOAT },
      { "a_vertexColor", Rendering::eDataType::VECTOR_4_FLOAT },
      { "a_vertexNorm", Rendering::eDataType::VECTOR_3_FLOAT },
      { "a_vertexTexCoord", Rendering::eDataType::VECTOR_2_FLOAT },
      { "a_instanceTransform", Rendering::eDataType::MATRIX_FLOAT_4X4 } });
}

// ----------------------------------------------------------------------------

Chimia::Rendering::ShaderInstance&
Chimia::Draw3D::Shaders::Generic()
{
  return generic;
}

// ----------------------------------------------------------------------------