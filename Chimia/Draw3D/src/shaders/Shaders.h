#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace Shaders {
void
Initialize();

Rendering::Shader&
VertexColored();

Rendering::Shader&
VertexColoredWithInstancedTransform();

Rendering::Shader&
GouraudLit();

Rendering::Shader&
GouraudLitWithInstancedTransformAndMaterial();

Rendering::Shader&
PhongLit();

Rendering::Shader&
PhongLitWithInstancedTransformAndMaterial();

Rendering::Shader&
GouraudLitWithVertexColor();

Rendering::Shader&
GouraudLitWithInstancedTransformAndVertexColor();

Rendering::Shader&
PhongLitWithVertexColor();

Rendering::Shader&
PhongLitWithInstancedTransformAndVertexColor();

Rendering::Shader&
Textured();

Rendering::Shader&
TexturedWithInstancedTransform();

Rendering::Shader&
ColoredTextured();

Rendering::Shader&
ColoredTexturedWithInstancedTransform();

Rendering::Shader&
GouraudLitTextured();

Rendering::Shader&
GouraudLitTexturedWithInstancedTransform();

Rendering::Shader&
PhongLitTextured();

Rendering::Shader&
PhongLitTexturedWithInstancedTransform();

Rendering::Shader&
GouraudLitColoredTextured();

Rendering::Shader&
GouraudLitColoredTexturedWithInstancedTransform();

Rendering::Shader&
PhongLitColoredTextured();

Rendering::Shader&
PhongLitColoredTexturedWithInstancedTransform();
}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
