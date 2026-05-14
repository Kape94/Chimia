#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// Vertex Position3 + Color4

void
Triangle(const VertexPC& v1, const VertexPC& v2, const VertexPC& v3);

void
Triangles(const std::vector<VertexPC>& vertices);

void
Triangles(const std::vector<VertexPC>& vertices,
          const std::vector<unsigned>& indices);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPC>& vertices);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPC>& vertices,
                     const std::vector<unsigned>& indices);

// Vertex Position3 + Normal3

void
Triangle(const VertexPN& v1,
         const VertexPN& v2,
         const VertexPN& v3,
         const MaterialID& material);

void
Triangles(const std::vector<VertexPN>& vertices, const MaterialID& material);

void
Triangles(const std::vector<VertexPN>& vertices,
          const std::vector<unsigned>& indices,
          const MaterialID& material);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPN>& vertices,
                     const MaterialID& material);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPN>& vertices,
                     const std::vector<unsigned>& indices,
                     const MaterialID& material);

// Vertex Position3 + TexCoord2

void
Triangle(const VertexPT& v1,
         const VertexPT& v2,
         const VertexPT& v3,
         const TextureID& texture);

void
Triangles(const std::vector<VertexPT>& vertices, const TextureID& texture);

void
Triangles(const std::vector<VertexPT>& vertices,
          const std::vector<unsigned>& indices,
          const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPT>& vertices,
                     const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPT>& vertices,
                     const std::vector<unsigned>& indices,
                     const TextureID& texture);

// Vertex Position3 + Color4 + Normal3

void
Triangle(const VertexPCN& v1, const VertexPCN& v2, const VertexPCN& v3);

void
Triangles(const std::vector<VertexPCN>& vertices);

void
Triangles(const std::vector<VertexPCN>& vertices,
          const std::vector<unsigned>& indices);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPCN>& vertices);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPCN>& vertices,
                     const std::vector<unsigned>& indices);

// Vertex Position3 + Normal3 + TexCoord2

void
Triangle(const VertexPNT& v1,
         const VertexPNT& v2,
         const VertexPNT& v3,
         const TextureID& texture);

void
Triangles(const std::vector<VertexPNT>& vertices, const TextureID& texture);

void
Triangles(const std::vector<VertexPNT>& vertices,
          const std::vector<unsigned>& indices,
          const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPNT>& vertices,
                     const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPNT>& vertices,
                     const std::vector<unsigned>& indices,
                     const TextureID& texture);

// Vertex Position3 + Color4 + TexCoord2

void
Triangle(const VertexPCT& v1,
         const VertexPCT& v2,
         const VertexPCT& v3,
         const TextureID& texture);

void
Triangles(const std::vector<VertexPCT>& vertices, const TextureID& texture);

void
Triangles(const std::vector<VertexPCT>& vertices,
          const std::vector<unsigned>& indices,
          const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPCT>& vertices,
                     const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPCT>& vertices,
                     const std::vector<unsigned>& indices,
                     const TextureID& texture);

// Vertex Position3 + Color4 + Normal3 + TexCoord2

void
Triangle(const VertexPCNT& v1,
         const VertexPCNT& v2,
         const VertexPCNT& v3,
         const TextureID& texture);

void
Triangles(const std::vector<VertexPCNT>& vertices, const TextureID& texture);

void
Triangles(const std::vector<VertexPCNT>& vertices,
          const std::vector<unsigned>& indices,
          const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPCNT>& vertices,
                     const TextureID& texture);

TriangleMeshID
AddRetainedTriangles(const std::vector<VertexPCNT>& vertices,
                     const std::vector<unsigned>& indices,
                     const TextureID& texture);

// General

void
DeleteRetainedTriangles(const TriangleMeshID& meshID);

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------