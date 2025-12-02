#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Core/DataBuffer.h"

#include "ObjectTable.h"
#include "Rendering/Buffer.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class StaticTriangles
{
public:
  StaticTriangles() = default;

  void Create(const size_t batchSize,
              const Rendering::ShaderAttributes& shaderAttributes);

  unsigned AddStaticMesh(const RawDataView& vertexData);

  void DeleteStaticMesh(const unsigned meshID);

  void Render();

private:
  bool CanRenderWithCurrentBuffer() const;

  bool HasSomethingToRender() const;

  void RebuildTrianglesBuffer();

  void RenderByBatches();

  void HandleRenderingForBatchRange(const size_t rangeStart,
                                    const size_t rangeSize);

  bool m_shouldRebuildBuffers = false;

  size_t m_batchSize = 0;

  Rendering::Buffer m_gpuBuffer;
  DataBuffer m_inputBuffer;
  ObjectTable<DataBuffer> m_staticTrianglesTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------