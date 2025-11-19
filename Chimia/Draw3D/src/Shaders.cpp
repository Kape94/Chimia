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