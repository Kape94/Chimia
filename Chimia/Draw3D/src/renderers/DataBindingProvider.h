#pragma once

//-----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "ShaderBindingsTemplate.h"
#include "Types.h"

#include "Rendering/DataLayout.h"
#include "Rendering/Target.h"

//-----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

//-----------------------------------------------------------------------------

class DataBindingProvider
{
public:
  DataBindingProvider() = default;
  DataBindingProvider(const eVertexLayout vertexLayout,
                      const Rendering::TargetInstance& target);

  const Rendering::DataLayout& GetVertexLayout() const;
  const Rendering::DataLayout& GetInstancedLayout() const;
  const Rendering::DataLayout& GetInstancedTransitionLayout() const;

  const ShaderBindingsTemplate& GetVertexTemplate() const;
  const ShaderBindingsTemplate& GetTargetVertexTemplate() const;
  const ShaderBindingsTemplate& GetInstancedTemplate() const;
  const ShaderBindingsTemplate& GetInstancedTransitionTemplate() const;

  const Rendering::TargetInstance& GetRenderingTarget() const;

private:
  Rendering::TargetInstance m_renderingTarget;

  Rendering::DataLayout m_vertexLayout;
  Rendering::DataLayout m_instancedLayout;
  Rendering::DataLayout m_instancedTransitionLayout;

  ShaderBindingsTemplate m_vertexTemplate;
  ShaderBindingsTemplate m_targetVertexTemplate;
  ShaderBindingsTemplate m_instancedTemplate;
  ShaderBindingsTemplate m_instancedTransitionTemplate;
};

//-----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

//-----------------------------------------------------------------------------