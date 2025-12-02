#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "StaticTriangles.h"
#include "TriangleBatch.h"

#include "Rendering/ShaderAttribute.h"

#include <initializer_list>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class TriangleMeshComponent
{
public:
  void Init(const size_t triangleBatchSize,
            const Rendering::ShaderAttributes& vertexAttributes,
            const std::function<void(void)>& onFlush);

  void Flush();

  void DrawTriangle(const std::initializer_list<RawDataView>& vertexData);
  void DrawTriangles(const RawArrayView& vertexDataArray);

  unsigned AddStaticMesh(const RawDataView& vertexData);
  void DeleteStaticMesh(const unsigned meshID);

private:
  Rendering::ShaderAttributes m_vertexAttributes;
  std::function<void(void)> m_onFlush;

  TriangleBatch m_triangleBatch;
  StaticTriangles m_staticTriangles;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------