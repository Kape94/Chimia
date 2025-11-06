#include "TriangleMeshComponent.h"
#include "Draw3DPrivate.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::Init(const size_t triangleBatchSize,
                            const Rendering::ShaderAttributes& vertexAttributes,
                            const std::function<void(void)>& onFlush)
{
  m_vertexAttributes = vertexAttributes;
  m_onFlush = onFlush;

  m_triangleBatch.Create(triangleBatchSize, vertexAttributes, onFlush);
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::Flush()
{
  m_onFlush();

  m_triangleBatch.Flush();
  m_staticTriangles.ForEach(
    [](const StaticTriangles& triangles) { triangles.Render(); });
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::DrawTriangle(
  const std::initializer_list<Bits::RawDataView>& vertexData)
{
  m_triangleBatch.Draw(vertexData);
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::DrawTriangles(const std::vector<float>& vertexData)
{
  m_triangleBatch.Draw({ vertexData.data(), vertexData.size(), sizeof(float) });
}

// ----------------------------------------------------------------------------

TriangleMeshID
TriangleMeshComponent::AddStaticMesh(const std::vector<float>& vertexData)
{
  auto [id, triangles] = m_staticTriangles.Insert();
  triangles->Create(vertexData, m_vertexAttributes);

  return Draw3DPrivate::CreateTriangleMeshID(id);
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::DeleteStaticMesh(const TriangleMeshID& meshID)
{
  m_staticTriangles.Delete(Draw3DPrivate::GetTriangleMeshIDValue(meshID));
}

// ----------------------------------------------------------------------------