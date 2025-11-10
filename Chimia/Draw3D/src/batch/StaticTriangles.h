#pragma once

#include "Bits/Buffer/RawBuffer.h"
#include "Draw3DNamespaceDefs.h"

#include "ObjectTable.h"
#include "Rendering/Buffer.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"

#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class StaticTriangles
{
public:
  StaticTriangles() = default;

  void Create(const size_t batchSize,
              const Rendering::ShaderAttributes& shaderAttributes);

  TriangleMeshID AddStaticMesh(const std::vector<float>& vertexData);

  void DeleteStaticMesh(const TriangleMeshID& meshID);

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
  Bits::RawBuffer m_inputBuffer;
  ObjectTable<Bits::RawBuffer> m_staticTrianglesTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------