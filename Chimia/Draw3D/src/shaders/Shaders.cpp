#include "Shaders.h"
// ----------------------------------------------------------------------------

#include "ShaderCodebase.h"

#include "glslCodes/Common.h"
#include "glslCodes/FragmentStage.h"
#include "glslCodes/InputLayout.h"
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

      // -------------------- InputLayout ------------------------

      { "vertexInputLayout::P3C3", ShaderCodes::VertexInputLayout::P3C3 },
      { "vertexInputLayout::P3C3Instanced",
        ShaderCodes::VertexInputLayout::P3C3Instanced },
      { "vertexInputLayout::P3N3", ShaderCodes::VertexInputLayout::P3N3 },
      { "vertexInputLayout::P3N3Instanced",
        ShaderCodes::VertexInputLayout::P3N3Instanced },
      { "vertexInputLayout::P3C3N3", ShaderCodes::VertexInputLayout::P3C3N3 },
      { "vertexInputLayout::P3C3N3Instanced",
        ShaderCodes::VertexInputLayout::P3C3N3Instanced },
      { "vertexInputLayout::P3T2", ShaderCodes::VertexInputLayout::P3T2 },
      { "vertexInputLayout::P3T2Instanced",
        ShaderCodes::VertexInputLayout::P3T2Instanced },
      { "vertexInputLayout::P3C3T2", ShaderCodes::VertexInputLayout::P3C3T2 },
      { "vertexInputLayout::P3C3T2Instanced",
        ShaderCodes::VertexInputLayout::P3C3T2Instanced },
      { "vertexInputLayout::P3N3T2", ShaderCodes::VertexInputLayout::P3N3T2 },
      { "vertexInputLayout::P3N3T2Instanced",
        ShaderCodes::VertexInputLayout::P3N3T2Instanced },
      { "vertexInputLayout::P3C3N3T2",
        ShaderCodes::VertexInputLayout::P3C3N3T2 },
      { "vertexInputLayout::P3C3N3T2Instanced",
        ShaderCodes::VertexInputLayout::P3C3N3T2Instanced },

      { "fragmentInputLayout::C3", ShaderCodes::FragmentInputLayout::C3 },
      { "fragmentInputLayout::C3T2", ShaderCodes::FragmentInputLayout::C3T2 },
      { "fragmentInputLayout::P3C3N3",
        ShaderCodes::FragmentInputLayout::P3C3N3 },
      { "fragmentInputLayout::P3C3N3T2",
        ShaderCodes::FragmentInputLayout::P3C3N3T2 },
      { "fragmentInputLayout::P3N3", ShaderCodes::FragmentInputLayout::P3N3 },
      { "fragmentInputLayout::P3N3T2",
        ShaderCodes::FragmentInputLayout::P3N3T2 },
      { "fragmentInputLayout::T2", ShaderCodes::FragmentInputLayout::T2 },
      { "fragmentInputLayout::T2Light",
        ShaderCodes::FragmentInputLayout::T2Light },

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