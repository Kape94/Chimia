#include "Triangle.h"

// ----------------------------------------------------------------------------

#include "Core/Types.h"
#include "Draw3DNamespaceDefs.h"
#include "Draw3DPrivate.h"
#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "Types.h"
#include "VertexColoredRendererImpl.h"
#include "eRendererType.h"
#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
auto& renderer = VertexColoredRendererImpl::getInstance();
auto& litRenderer = LitRendererImpl::getInstance();
auto& litVertexColoredRenderer = LitWithVertexColorRendererImpl::getInstance();

template<class VertexType>
std::vector<VertexType>
DropIndices(const std::vector<VertexType>& vertexData,
            const std::vector<unsigned>& indices)
{
  std::vector<VertexType> unindexedData;
  unindexedData.reserve(indices.size());

  for (const unsigned i : indices) {
    unindexedData.push_back(vertexData[i]);
  }

  return unindexedData;
}

}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPC& v1,
                         const VertexPC& v2,
                         const VertexPC& v3)
{
  renderer.DrawTriangle(
    v1.position, v1.color, v2.position, v2.color, v3.position, v3.color);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPC>& vertices)
{
  renderer.DrawTriangles(
    { vertices.data(), vertices.size(), sizeof(VertexPC) });
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPC>& vertices,
                          const std::vector<unsigned>& indices)
{
  const std::vector<VertexPC> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPC>& vertices)
{
  return renderer.AddStaticTriangles(
    RawDataView(vertices.data(), vertices.size() * sizeof(VertexPC)));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPC>& vertices,
                                   const std::vector<unsigned>& indices)
{
  const std::vector<VertexPC> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPN& v1,
                         const VertexPN& v2,
                         const VertexPN& v3,
                         const MaterialID& material)
{
  litRenderer.DrawTriangle(v1.position,
                           v1.normal,
                           v2.position,
                           v2.normal,
                           v3.position,
                           v3.normal,
                           material);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPN>& vertices,
                          const MaterialID& material)
{
  litRenderer.DrawTriangles(
    { vertices.data(), vertices.size(), sizeof(VertexPN) }, material);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPN>& vertices,
                          const std::vector<unsigned>& indices,
                          const MaterialID& material)
{
  const std::vector<VertexPN> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex, material);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPN>& vertices,
                                   const MaterialID& material)
{
  return litRenderer.AddStaticTriangles(
    RawDataView(vertices.data(), vertices.size() * sizeof(VertexPN)), material);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPN>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const MaterialID& material)
{
  const std::vector<VertexPN> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, material);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPCN& v1,
                         const VertexPCN& v2,
                         const VertexPCN& v3)
{
  litVertexColoredRenderer.DrawTriangle(v1.position,
                                        v1.color,
                                        v1.normal,
                                        v2.position,
                                        v2.color,
                                        v2.normal,
                                        v3.position,
                                        v3.color,
                                        v3.normal);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCN>& vertices)
{
  renderer.DrawTriangles(
    { vertices.data(), vertices.size(), sizeof(VertexPCN) });
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCN>& vertices,
                          const std::vector<unsigned>& indices)
{
  const std::vector<VertexPCN> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCN>& vertices)
{
  return renderer.AddStaticTriangles(
    RawDataView(vertices.data(), vertices.size() * sizeof(VertexPCN)));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCN>& vertices,
                                   const std::vector<unsigned>& indices)
{
  const std::vector<VertexPCN> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  auto [rendererID, _, __] = Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  const auto rendererType = static_cast<eRendererType>(rendererID);
  switch (rendererType) {
    case eRendererType::VERTEX_COLORED: {
      renderer.DeleteStaticTriangles(meshID);
      return;
    }
    case eRendererType::LIT: {
      litRenderer.DeleteStaticTriangles(meshID);
      return;
    }
    case eRendererType::VERTEX_COLORED_LIT: {
      litVertexColoredRenderer.DeleteStaticTriangles(meshID);
      return;
    }
    case eRendererType::NONE:
    default:
      return;
  }
}

// ----------------------------------------------------------------------------