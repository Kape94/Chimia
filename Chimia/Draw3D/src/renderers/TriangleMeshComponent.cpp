#include "TriangleMeshComponent.h"

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
  m_staticTriangles.Create(triangleBatchSize, vertexAttributes);
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::Flush()
{
  m_onFlush();

  m_triangleBatch.Flush();
  m_staticTriangles.Render();
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::DrawTriangle(
  const std::initializer_list<RawDataView>& vertexData)
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

void
TriangleMeshComponent::DrawTriangles(const std::vector<float>& vertexData,
                                     const std::vector<unsigned>& indices)
{
  const std::vector<float> unindexedVertexData =
    DropIndices(vertexData, indices);

  DrawTriangles(unindexedVertexData);
}

// ----------------------------------------------------------------------------

unsigned
TriangleMeshComponent::AddStaticMesh(const std::vector<float>& vertexData)
{
  return m_staticTriangles.AddStaticMesh(vertexData);
}

// ----------------------------------------------------------------------------

unsigned
TriangleMeshComponent::AddStaticMesh(const std::vector<float>& vertexData,
                                     const std::vector<unsigned>& indices)
{
  const std::vector<float> unindexedVertexData =
    DropIndices(vertexData, indices);

  return m_staticTriangles.AddStaticMesh(unindexedVertexData);
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::DeleteStaticMesh(const unsigned meshID)
{
  m_staticTriangles.DeleteStaticMesh(meshID);
}

// ----------------------------------------------------------------------------

std::vector<float>
TriangleMeshComponent::DropIndices(const std::vector<float>& vertexData,
                                   const std::vector<unsigned>& indices)
{
  std::vector<float> unindexedData;

  const size_t vertexSize = m_vertexAttributes.ComputeTotalSizeOfAttributes();
  const size_t vertexFloatSize = vertexSize / sizeof(float);

  unindexedData.reserve(vertexFloatSize * indices.size());
  for (const unsigned i : indices) {
    const size_t offset = i * vertexFloatSize;
    const auto vertexDataStart = vertexData.begin() + offset;
    const auto vertexDataEnd = vertexDataStart + vertexFloatSize;
    unindexedData.insert(unindexedData.end(), vertexDataStart, vertexDataEnd);
  }

  return unindexedData;
}

// ----------------------------------------------------------------------------