#include "Triangle.h"

// ----------------------------------------------------------------------------

#include "Core/Types.h"
#include "Draw3DNamespaceDefs.h"
#include "Draw3DPrivate.h"
#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "TexturedLitRendererImpl.h"
#include "TexturedRendererImpl.h"
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
auto& texturedRenderer = TexturedRendererImpl::getInstance();
auto& texturedLitRenderer = TexturedLitRendererImpl::getInstance();

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

template<typename T>
Chimia::RawArrayView
VectorArrayView(const std::vector<T>& vector)
{
  return { vector.data(), vector.size(), sizeof(T) };
}

template<typename T>
Chimia::RawDataView
VectorDataView(const std::vector<T>& vector)
{
  return { vector.data(), vector.size() * sizeof(T) };
}
}

// ----------------------------------------------------------------------------
// Position3 + Color3
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
  renderer.DrawTriangles(VectorArrayView(vertices));
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
  return renderer.AddStaticTriangles(VectorDataView(vertices));
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
// Position3 + Normal3
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
  litRenderer.DrawTriangles(VectorArrayView(vertices), material);
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
  return litRenderer.AddStaticTriangles(VectorDataView(vertices), material);
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
// Position3 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPT& v1,
                         const VertexPT& v2,
                         const VertexPT& v3,
                         const TextureID& texture)
{
  texturedRenderer.DrawTriangle(v1.position,
                                v1.texCoord,
                                v2.position,
                                v2.texCoord,
                                v3.position,
                                v3.texCoord,
                                texture);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPT>& vertices,
                          const TextureID& texture)
{
  texturedRenderer.DrawTriangles(VectorArrayView(vertices), texture);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPT>& vertices,
                          const std::vector<unsigned>& indices,
                          const TextureID& texture)
{
  const std::vector<VertexPT> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPT>& vertices,
                                   const TextureID& texture)
{
  return texturedRenderer.AddStaticTriangles(VectorDataView(vertices), texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPT>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const TextureID& texture)
{
  const std::vector<VertexPT> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// Position3 + Color3 + Normal3
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
  renderer.DrawTriangles(VectorArrayView(vertices));
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
  return renderer.AddStaticTriangles(VectorDataView(vertices));
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
// Position3 + Normal3 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPNT& v1,
                         const VertexPNT& v2,
                         const VertexPNT& v3,
                         const TextureID& texture)
{
  texturedLitRenderer.DrawTriangle(v1.position,
                                   v1.normal,
                                   v1.texCoord,
                                   v2.position,
                                   v2.normal,
                                   v2.texCoord,
                                   v3.position,
                                   v3.normal,
                                   v3.texCoord,
                                   texture);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPNT>& vertices,
                          const TextureID& texture)
{
  texturedLitRenderer.DrawTriangles(VectorArrayView(vertices), texture);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPNT>& vertices,
                          const std::vector<unsigned>& indices,
                          const TextureID& texture)
{
  const std::vector<VertexPNT> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPNT>& vertices,
                                   const TextureID& texture)
{
  return texturedLitRenderer.AddStaticTriangles(VectorDataView(vertices),
                                                texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPNT>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const TextureID& texture)
{
  const std::vector<VertexPNT> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// General
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  auto [rendererID, _, __, ___] =
    Draw3DPrivate::GetTriangleMeshIDValues(meshID);

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
    case eRendererType::TEXTURED: {
      texturedRenderer.DeleteStaticTriangles(meshID);
      return;
    }
    case eRendererType::VERTEX_COLORED_LIT: {
      litVertexColoredRenderer.DeleteStaticTriangles(meshID);
      return;
    }
    case eRendererType::TEXTURED_LIT: {
      texturedLitRenderer.DeleteStaticTriangles(meshID);
      return;
    }
    case eRendererType::NONE:
    default:
      return;
  }
}

// ----------------------------------------------------------------------------