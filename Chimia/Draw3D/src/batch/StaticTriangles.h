#pragma once

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "Core/DataBuffer.h"
#include "Types.h"

#include "ObjectTable.h"
#include "Rendering/Buffer.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class StaticTriangles
{
public:
  DEFAULT_CONSTUCTIBLE(StaticTriangles)
  NON_COPYABLE_NON_MOVABLE(StaticTriangles)

  void Create(const BatchingSettings& batchingSettings,
              const Rendering::ShaderAttributes& shaderAttributes);

  unsigned AddStaticMesh(const RawDataView& vertexData);

  void DeleteStaticMesh(const unsigned meshID);

  void Render();

private:
  bool CanRenderWithCurrentBuffer() const;

  bool HasSomethingToRender() const;

  void RebuildTrianglesBuffer();

  void HandleDynamicResizing();

  void ResizeGPUBatch(const size_t batchSize);

  void RenderByBatches();

  void HandleRenderingForBatchRange(const size_t rangeStart,
                                    const size_t rangeSize);

  size_t CurrentGPUBatchSizeInBytes() const;

  // Cache attribute to indicate whenever a new mesh gets added
  bool m_shouldRebuildBuffers = false;

  // Fixed attributes, only set during initialization
  BatchingSettings m_batchingSettings;
  Rendering::ShaderAttributes m_vertexAttributes;
  size_t m_triangleSizeInBytes = 0;

  // These attributes get modified as rendering requests come in
  size_t m_currentGPUBatchSize = 0;
  Rendering::Buffer m_gpuBuffer;
  DataBuffer m_inputBuffer;
  ObjectTable<DataBuffer> m_staticTrianglesTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------