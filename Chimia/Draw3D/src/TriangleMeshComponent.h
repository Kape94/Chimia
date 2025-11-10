#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "StaticTriangles.h"
#include "TriangleBatch.h"
#include "Types.h"

#include "Bits/Buffer/RawDataView.h"
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

  void DrawTriangles(const std::vector<float>& vertexData);
  void DrawTriangle(const std::initializer_list<Bits::RawDataView>& vertexData);

  TriangleMeshID AddStaticMesh(const std::vector<float>& vertexData);

  void DeleteStaticMesh(const TriangleMeshID& meshID);

private:
  Rendering::ShaderAttributes m_vertexAttributes;
  std::function<void(void)> m_onFlush;

  TriangleBatch m_triangleBatch;
  StaticTriangles m_staticTriangles;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------