#include "Shaders.h"
// ----------------------------------------------------------------------------

#include "ShaderCodebase.h"
#include "ShaderCodes.h"

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
Chimia::Rendering::Shader textured;
Chimia::Rendering::Shader texturedWithInstancedTransform;

void
InitializeCodebase()
{
  ShaderCodebase::RegisterCodes({
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
    { "vertex::textured", ShaderCodes::Vertex::textured },
    { "vertex::texturedWithInstancedTransform",
      ShaderCodes::Vertex::texturedWithInstancedTransform },

    // ----------------------- Fragment --------------------------

    { "fragment::colored", ShaderCodes::Fragment::colored },
    { "fragment::gouraudLit", ShaderCodes::Fragment::gouraudLit },
    { "fragment::phongLit", ShaderCodes::Fragment::phongLit },
    { "fragment::phongLitWithInstancedTransformAndMaterial",
      ShaderCodes::Fragment::phongLitWithInstancedTransformAndMaterial },
    { "fragment::textured", ShaderCodes::Fragment::textured },
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

  textured.Create(ShaderCodebase::Code("vertex::textured"),
                  ShaderCodebase::Code("fragment::textured"));

  texturedWithInstancedTransform.Create(
    ShaderCodebase::Code("vertex::texturedWithInstancedTransform"),
    ShaderCodebase::Code("fragment::textured"));
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