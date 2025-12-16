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
Chimia::Rendering::Shader vertexColored;
Chimia::Rendering::Shader vertexColoredWithInstancedTransform;
Chimia::Rendering::Shader gouraudLit;
Chimia::Rendering::Shader gouraudLitWithInstancedTransformAndMaterial;
Chimia::Rendering::Shader phongLit;
Chimia::Rendering::Shader phongLitWithInstancedTransformAndMaterial;
Chimia::Rendering::Shader gouraudLitWithVertexColor;
Chimia::Rendering::Shader gouraudLitWithInstancedTransformAndVertexColor;
Chimia::Rendering::Shader phongLitWithVertexColor;
Chimia::Rendering::Shader phongLitWithInstancedTransformAndVertexColor;
Chimia::Rendering::Shader textured;
Chimia::Rendering::Shader texturedWithInstancedTransform;
Chimia::Rendering::Shader coloredTextured;
Chimia::Rendering::Shader coloredTexturedWithInstancedTransform;
Chimia::Rendering::Shader gouraudLitTextured;
Chimia::Rendering::Shader gouraudLitTexturedWithInstancedTransform;
Chimia::Rendering::Shader phongLitTextured;
Chimia::Rendering::Shader phongLitTexturedWithInstancedTransform;
Chimia::Rendering::Shader gouraudLitColoredTextured;
Chimia::Rendering::Shader gouraudLitColoredTexturedWithInstancedTransform;
Chimia::Rendering::Shader phongLitColoredTextured;
Chimia::Rendering::Shader phongLitColoredTexturedWithInstancedTransform;

void
InitializeCodebase()
{
  ShaderCodebase::AddCodes({
    // ----------------------- Common --------------------------

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
    { "vertexInputLayout::P3C3N3T2", ShaderCodes::VertexInputLayout::P3C3N3T2 },
    { "vertexInputLayout::P3C3N3T2Instanced",
      ShaderCodes::VertexInputLayout::P3C3N3T2Instanced },

    { "fragmentInputLayout::C3", ShaderCodes::FragmentInputLayout::C3 },
    { "fragmentInputLayout::C3T2", ShaderCodes::FragmentInputLayout::C3T2 },
    { "fragmentInputLayout::P3C3N3", ShaderCodes::FragmentInputLayout::P3C3N3 },
    { "fragmentInputLayout::P3C3N3T2",
      ShaderCodes::FragmentInputLayout::P3C3N3T2 },
    { "fragmentInputLayout::P3N3", ShaderCodes::FragmentInputLayout::P3N3 },
    { "fragmentInputLayout::P3N3T2", ShaderCodes::FragmentInputLayout::P3N3T2 },
    { "fragmentInputLayout::T2", ShaderCodes::FragmentInputLayout::T2 },
    { "fragmentInputLayout::T2Light",
      ShaderCodes::FragmentInputLayout::T2Light },

    // ----------------------- Vertex --------------------------

    { "vertex::colored", ShaderCodes::Vertex::colored },
    { "vertex::coloredWithInstancedTransform",
      ShaderCodes::Vertex::coloredWithInstancedTransform },
    { "vertex::gouraudLit", ShaderCodes::Vertex::gouraudLit },
    { "vertex::gouraudLitWithInstancedTransformAndMaterial",
      ShaderCodes::Vertex::gouraudLitWithInstancedTransformAndMaterial },
    { "vertex::phongLit", ShaderCodes::Vertex::phongLit },
    { "vertex::phongLitWithInstancedTransformAndMaterial",
      ShaderCodes::Vertex::phongLitWithInstancedTransformAndMaterial },
    { "vertex::gouraudLitWithVertexColor",
      ShaderCodes::Vertex::gouraudLitWithVertexColor },
    { "vertex::gouraudLitWithInstancedTransformAndVertexColor",
      ShaderCodes::Vertex::gouraudLitWithInstancedTransformAndVertexColor },
    { "vertex::phongLitWithVertexColor",
      ShaderCodes::Vertex::phongLitWithVertexColor },
    { "vertex::phongLitWithInstancedTransformAndVertexColor",
      ShaderCodes::Vertex::phongLitWithInstancedTransformAndVertexColor },
    { "vertex::textured", ShaderCodes::Vertex::textured },
    { "vertex::texturedWithInstancedTransform",
      ShaderCodes::Vertex::texturedWithInstancedTransform },
    { "vertex::coloredTextured", ShaderCodes::Vertex::coloredTextured },
    { "vertex::coloredTexturedWithInstancedTransform",
      ShaderCodes::Vertex::coloredTexturedWithInstancedTransform },
    { "vertex::gouraudLitTextured", ShaderCodes::Vertex::gouraudLitTextured },
    { "vertex::gouraudLitTexturedWithInstancedTransform",
      ShaderCodes::Vertex::gouraudLitTexturedWithInstancedTransform },
    { "vertex::phongLitTextured", ShaderCodes::Vertex::phongLitTextured },
    { "vertex::phongLitTexturedWithInstancedTransform",
      ShaderCodes::Vertex::phongLitTexturedWithInstancedTransform },
    { "vertex::gouraudLitColoredTextured",
      ShaderCodes::Vertex::gouraudLitColoredTextured },
    { "vertex::gouraudLitColoredTexturedWithInstancedTransform",
      ShaderCodes::Vertex::gouraudLitColoredTexturedWithInstancedTransform },
    { "vertex::phongLitColoredTextured",
      ShaderCodes::Vertex::phongLitColoredTextured },
    { "vertex::phongLitColoredTexturedWithInstancedTransform",
      ShaderCodes::Vertex::phongLitColoredTexturedWithInstancedTransform },

    // ----------------------- Fragment --------------------------

    { "fragment::colored", ShaderCodes::Fragment::colored },
    { "fragment::gouraudLit", ShaderCodes::Fragment::gouraudLit },
    { "fragment::phongLit", ShaderCodes::Fragment::phongLit },
    { "fragment::phongLitWithInstancedTransformAndMaterial",
      ShaderCodes::Fragment::phongLitWithInstancedTransformAndMaterial },
    { "fragment::phongLitByVertexColor",
      ShaderCodes::Fragment::phongLitByVertexColor },
    { "fragment::textured", ShaderCodes::Fragment::textured },
    { "fragment::coloredTextured", ShaderCodes::Fragment::coloredTextured },
    { "fragment::gouraudLitTextured",
      ShaderCodes::Fragment::gouraudLitTextured },
    { "fragment::phongLitTextured", ShaderCodes::Fragment::phongLitTextured },
    { "fragment::phongLitColoredTextured",
      ShaderCodes::Fragment::phongLitColoredTextured },
  });
}

}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Shaders::Initialize()
{
  InitializeCodebase();

  vertexColored.Create(ShaderCodebase::Code("vertex::colored"),
                       ShaderCodebase::Code("fragment::colored"));

  vertexColoredWithInstancedTransform.Create(
    ShaderCodebase::Code("vertex::coloredWithInstancedTransform"),
    ShaderCodebase::Code("fragment::colored"));

  gouraudLit.Create(ShaderCodebase::Code("vertex::gouraudLit"),
                    ShaderCodebase::Code("fragment::gouraudLit"));

  gouraudLitWithInstancedTransformAndMaterial.Create(
    ShaderCodebase::Code("vertex::gouraudLitWithInstancedTransformAndMaterial"),
    ShaderCodebase::Code("fragment::gouraudLit"));

  phongLit.Create(ShaderCodebase::Code("vertex::phongLit"),
                  ShaderCodebase::Code("fragment::phongLit"));

  phongLitWithInstancedTransformAndMaterial.Create(
    ShaderCodebase::Code("vertex::phongLitWithInstancedTransformAndMaterial"),
    ShaderCodebase::Code(
      "fragment::phongLitWithInstancedTransformAndMaterial"));

  gouraudLitWithVertexColor.Create(
    ShaderCodebase::Code("vertex::gouraudLitWithVertexColor"),
    ShaderCodebase::Code("fragment::gouraudLit"));

  gouraudLitWithInstancedTransformAndVertexColor.Create(
    ShaderCodebase::Code(
      "vertex::gouraudLitWithInstancedTransformAndVertexColor"),
    ShaderCodebase::Code("fragment::gouraudLit"));

  phongLitWithVertexColor.Create(
    ShaderCodebase::Code("vertex::phongLitWithVertexColor"),
    ShaderCodebase::Code("fragment::phongLitByVertexColor"));

  phongLitWithInstancedTransformAndVertexColor.Create(
    ShaderCodebase::Code(
      "vertex::phongLitWithInstancedTransformAndVertexColor"),
    ShaderCodebase::Code("fragment::phongLitByVertexColor"));

  textured.Create(ShaderCodebase::Code("vertex::textured"),
                  ShaderCodebase::Code("fragment::textured"));

  texturedWithInstancedTransform.Create(
    ShaderCodebase::Code("vertex::texturedWithInstancedTransform"),
    ShaderCodebase::Code("fragment::textured"));

  coloredTextured.Create(ShaderCodebase::Code("vertex::coloredTextured"),
                         ShaderCodebase::Code("fragment::coloredTextured"));

  coloredTexturedWithInstancedTransform.Create(
    ShaderCodebase::Code("vertex::coloredTexturedWithInstancedTransform"),
    ShaderCodebase::Code("fragment::coloredTextured"));

  gouraudLitTextured.Create(
    ShaderCodebase::Code("vertex::gouraudLitTextured"),
    ShaderCodebase::Code("fragment::gouraudLitTextured"));

  gouraudLitTexturedWithInstancedTransform.Create(
    ShaderCodebase::Code("vertex::gouraudLitTexturedWithInstancedTransform"),
    ShaderCodebase::Code("fragment::gouraudLitTextured"));

  phongLitTextured.Create(ShaderCodebase::Code("vertex::phongLitTextured"),
                          ShaderCodebase::Code("fragment::phongLitTextured"));

  phongLitTexturedWithInstancedTransform.Create(
    ShaderCodebase::Code("vertex::phongLitTexturedWithInstancedTransform"),
    ShaderCodebase::Code("fragment::phongLitTextured"));

  gouraudLitColoredTextured.Create(
    ShaderCodebase::Code("vertex::gouraudLitColoredTextured"),
    ShaderCodebase::Code("fragment::gouraudLitTextured"));

  gouraudLitColoredTexturedWithInstancedTransform.Create(
    ShaderCodebase::Code(
      "vertex::gouraudLitColoredTexturedWithInstancedTransform"),
    ShaderCodebase::Code("fragment::gouraudLitTextured"));

  phongLitColoredTextured.Create(
    ShaderCodebase::Code("vertex::phongLitColoredTextured"),
    ShaderCodebase::Code("fragment::phongLitColoredTextured"));

  phongLitColoredTexturedWithInstancedTransform.Create(
    ShaderCodebase::Code(
      "vertex::phongLitColoredTexturedWithInstancedTransform"),
    ShaderCodebase::Code("fragment::phongLitColoredTextured"));
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::VertexColored()
{
  return vertexColored;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::VertexColoredWithInstancedTransform()
{
  return vertexColoredWithInstancedTransform;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLit()
{
  return gouraudLit;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLitWithInstancedTransformAndMaterial()
{
  return gouraudLitWithInstancedTransformAndMaterial;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLit()
{
  return phongLit;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLitWithInstancedTransformAndMaterial()
{
  return phongLitWithInstancedTransformAndMaterial;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLitWithVertexColor()
{
  return gouraudLitWithVertexColor;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLitWithInstancedTransformAndVertexColor()
{
  return gouraudLitWithInstancedTransformAndVertexColor;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLitWithVertexColor()
{
  return phongLitWithVertexColor;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLitWithInstancedTransformAndVertexColor()
{
  return phongLitWithInstancedTransformAndVertexColor;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::Textured()
{
  return textured;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::TexturedWithInstancedTransform()
{
  return texturedWithInstancedTransform;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::ColoredTextured()
{
  return coloredTextured;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::ColoredTexturedWithInstancedTransform()
{
  return coloredTexturedWithInstancedTransform;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLitTextured()
{
  return gouraudLitTextured;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLitTexturedWithInstancedTransform()
{
  return gouraudLitTexturedWithInstancedTransform;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLitTextured()
{
  return phongLitTextured;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLitTexturedWithInstancedTransform()
{
  return phongLitTexturedWithInstancedTransform;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLitColoredTextured()
{
  return gouraudLitColoredTextured;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::GouraudLitColoredTexturedWithInstancedTransform()
{
  return gouraudLitColoredTexturedWithInstancedTransform;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLitColoredTextured()
{
  return phongLitColoredTextured;
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Shader&
Chimia::Draw3D::Shaders::PhongLitColoredTexturedWithInstancedTransform()
{
  return phongLitColoredTexturedWithInstancedTransform;
}

// ----------------------------------------------------------------------------