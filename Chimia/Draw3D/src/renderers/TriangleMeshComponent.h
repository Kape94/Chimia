#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "ImmediateTrianglesBatch.h"
#include "RetainedTrianglesBatch.h"

#include <initializer_list>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class TriangleMeshComponent
{
public:
  DEFAULT_CONSTUCTIBLE(TriangleMeshComponent)
  NON_COPYABLE_NON_MOVABLE(TriangleMeshComponent)

  void Init(const BatchingSettings& batchingSettings,
            const Rendering::DataLayout& vertexDataLayout,
            const Rendering::ShaderBindingsTemplate& vertexBindingsTemplates,
            const std::function<void(void)>& onFlush);

  void Flush(const eImmediateFlusingPolicy flushingPolicy);

  void DrawTriangle(const std::initializer_list<RawDataView>& vertexData);
  void DrawTriangles(const RawArrayView& vertexDataArray);

  unsigned AddRetainedMesh(const RawDataView& vertexData);
  void DeleteRetainedMesh(const unsigned meshID);

private:
  std::function<void(void)> m_onFlush;

  ImmediateTrianglesBatch m_triangleBatch;
  RetainedTrianglesBatch m_staticTriangles;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------