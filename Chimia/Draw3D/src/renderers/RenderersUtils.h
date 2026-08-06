#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Rendering/DataLayout.h"
#include "Rendering/Target.h"
#include "ResourceGroup.h"
#include "ShaderBindingsTemplate.h"
#include "Types.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

struct VertexLayoutDataSchemas
{
  Chimia::Rendering::DataLayout vertexDataLayout;
  Chimia::Rendering::DataLayout instancedDataLayout;
};

struct VertexLayoutBindingsTemplates
{
  ShaderBindingsTemplate vertexBindingsTemplate;
  ShaderBindingsTemplate instancedBindingsTemplate;
};

// ----------------------------------------------------------------------------

namespace RenderersUtils {

VertexLayoutDataSchemas
GetDataSchemasForLayout(const eVertexLayout& layout);

VertexLayoutBindingsTemplates
GetBindingsTemplatesForLayout(const eVertexLayout& layout,
                              const Rendering::TargetInstance& target);

const Rendering::TargetInstance&
GetDefaultRenderingTarget();

Chimia::Rendering::DataLayout
GetVertexDataSchema(const eVertexLayout& layout);

void
ConfigureShaderForRendering(Rendering::ShaderInstance& shader,
                            const eVertexLayout& layout,
                            const ResourcesGroup& resources);

void
ConfigureShaderForInstancedRendering(Rendering::ShaderInstance& shader,
                                     const eVertexLayout& layout,
                                     const ResourcesGroup& resources);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------