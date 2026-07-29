#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Rendering/DataLayout.h"
#include "ResourceGroup.h"
#include "Types.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

struct VertexLayoutAttributes
{
  Chimia::Rendering::ShaderAttributes vertexAttributes;
  Chimia::Rendering::ShaderAttributes instancedAttributes;
};

struct VertexLayoutDataSchemas
{
  Chimia::Rendering::DataLayout vertexDataLayout;
  Chimia::Rendering::DataLayout instancedDataLayout;
};

namespace RenderersUtils {

VertexLayoutAttributes
GetAttributesForLayout(const eVertexLayout& layout);

VertexLayoutDataSchemas
GetDataSchemasForLayout(const eVertexLayout& layout);

Chimia::Rendering::DataLayout
GetVertexDataSchema(const eVertexLayout& layout);

void
ConfigureShaderForRendering(Rendering::Shader& shader,
                            const eVertexLayout& layout,
                            const ResourcesGroup& resources);

void
ConfigureShaderForInstancedRendering(Rendering::Shader& shader,
                                     const eVertexLayout& layout,
                                     const ResourcesGroup& resources);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------