#include "Shaders.h"
// ----------------------------------------------------------------------------

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
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Shaders::Initialize()
{
  vertexColored.Create(ShaderCodes::Vertex::colored,
                       ShaderCodes::Fragment::colored);

  vertexColoredWithInstancedTransform.Create(
    ShaderCodes::Vertex::coloredWithInstancedTransform,
    ShaderCodes::Fragment::colored);

  gouraudLit.Create(ShaderCodes::Vertex::gouraudLit,
                    ShaderCodes::Fragment::gouraudLit);

  gouraudLitWithInstancedTransformAndMaterial.Create(
    ShaderCodes::Vertex::gouraudLitWithInstancedTransformAndMaterial,
    ShaderCodes::Fragment::gouraudLit);

  phongLit.Create(ShaderCodes::Vertex::phongLit,
                  ShaderCodes::Fragment::phongLit);

  phongLitWithInstancedTransformAndMaterial.Create(
    ShaderCodes::Vertex::phongLitWithInstancedTransformAndMaterial,
    ShaderCodes::Fragment::phongLitWithInstancedTransformAndMaterial);

  gouraudLitWithVertexColor.Create(
    ShaderCodes::Vertex::gouraudLitWithVertexColor,
    ShaderCodes::Fragment::gouraudLit);

  gouraudLitWithInstancedTransformAndVertexColor.Create(
    ShaderCodes::Vertex::gouraudLitWithInstancedTransformAndVertexColor,
    ShaderCodes::Fragment::gouraudLit);

  textured.Create(ShaderCodes::Vertex::textured,
                  ShaderCodes::Fragment::textured);

  texturedWithInstancedTransform.Create(
    ShaderCodes::Vertex::texturedWithInstancedTransform,
    ShaderCodes::Fragment::textured);
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