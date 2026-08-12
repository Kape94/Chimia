#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "ImmediateTransitionsBatch.h"
#include "ObjectTable.h"
#include "Rendering/DataLayout.h"
#include "ShaderBindingsTemplate.h"
#include "Types.h"

#include <glm/ext/matrix_float4x4.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class TransitionRenderingComponent
{
public:
  DEFAULT_CONSTUCTIBLE(TransitionRenderingComponent)
  NON_COPYABLE_NON_MOVABLE(TransitionRenderingComponent)

  void Init(const BatchingSettings& batchingSettings,
            const Rendering::DataLayout& instancedDataLayout,
            const ShaderBindingsTemplate& startBindingsTemplates,
            const ShaderBindingsTemplate& targetBindingsTemplates,
            const ShaderBindingsTemplate& instancedBindingsTemplates,
            const std::function<void(void)>& onFlush);

  void Flush(const eImmediateFlusingPolicy flushingPolicy);

  void DrawTransition(const TransitionID& transition,
                      const RawDataView& instanceData);
  void DrawTransition(const TransitionID& transition,
                      const std::initializer_list<RawDataView>& instanceDatas);

private:
  ImmediateTransitionsBatch* AllocateImmediateBatchForTransition(
    const TransitionID& transitionID);

  BatchingSettings m_batchingSettings;
  Rendering::DataLayout m_vertexDataLayout;
  Rendering::DataLayout m_instancedDataLayout;
  ShaderBindingsTemplate m_startBindingsTemplates;
  ShaderBindingsTemplate m_targetBindingsTemplates;
  ShaderBindingsTemplate m_instancedBindingsTemplates;
  std::function<void(void)> m_onFlush;

  ObjectTable<ImmediateTransitionsBatch> m_transitionsTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------