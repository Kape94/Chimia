#include "TriangleMeshComponent.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::Init(
  const BatchingSettings& batchingSettings,
  const Rendering::DataLayout& vertexDataLayout,
  const ShaderBindingsTemplate& vertexBindingsTemplates,
  const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;

  m_triangleBatch.Create(
    batchingSettings, vertexDataLayout, vertexBindingsTemplates, onFlush);
  m_staticTriangles.Create(
    batchingSettings, vertexDataLayout, vertexBindingsTemplates);
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::Flush(const eImmediateFlusingPolicy flushingPolicy)
{
  m_onFlush();

  m_triangleBatch.Flush(flushingPolicy);
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
TriangleMeshComponent::DrawTriangles(const RawArrayView& vertexDataArray)
{
  m_triangleBatch.Draw(vertexDataArray);
}

// ----------------------------------------------------------------------------

unsigned
TriangleMeshComponent::AddRetainedMesh(const RawDataView& vertexData)
{
  return m_staticTriangles.AddStaticMesh(vertexData);
}

// ----------------------------------------------------------------------------

void
TriangleMeshComponent::DeleteRetainedMesh(const unsigned meshID)
{
  m_staticTriangles.DeleteStaticMesh(meshID);
}

// ----------------------------------------------------------------------------
