#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "ImmediateTrianglesBatch.h"
#include "RetainedTrianglesBatch.h"

#include "Rendering/ShaderAttribute.h"

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
            const Rendering::ShaderAttributes& vertexAttributes,
            const std::function<void(void)>& onFlush);

  void Flush();

  void DrawTriangle(const std::initializer_list<RawDataView>& vertexData);
  void DrawTriangles(const RawArrayView& vertexDataArray);

  unsigned AddStaticMesh(const RawDataView& vertexData);
  void DeleteStaticMesh(const unsigned meshID);

private:
  std::function<void(void)> m_onFlush;

  ImmediateTrianglesBatch m_triangleBatch;
  RetainedTrianglesBatch m_staticTriangles;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------