#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Rendering/Target.h"
#include "ResourceGroup.h"
#include "Types.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace RenderersUtils {

const Rendering::TargetInstance&
GetDefaultRenderingTarget();

bool
HasColor(const eVertexLayout& layout);

bool
HasNormal(const eVertexLayout& layout);

bool
HasTexCoord(const eVertexLayout& layout);

void
ConfigureShaderForRendering(Rendering::ShaderInstance& shader,
                            const eVertexLayout& layout,
                            const ResourcesGroup& resources);

void
ConfigureShaderForInstancedRendering(Rendering::ShaderInstance& shader,
                                     const eVertexLayout& layout,
                                     const ResourcesGroup& resources);

void
ConfigureForTransitionRendering(Rendering::ShaderInstance& shader);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------