#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// Vertex Position3 + Color3

void
Triangle(const VertexPC& v1, const VertexPC& v2, const VertexPC& v3);

void
Triangles(const std::vector<VertexPC>& vertices);

void
Triangles(const std::vector<VertexPC>& vertices,
          const std::vector<unsigned>& indices);

TriangleMeshID
AddStaticTriangles(const std::vector<VertexPC>& vertices);

TriangleMeshID
AddStaticTriangles(const std::vector<VertexPC>& vertices,
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
AddStaticTriangles(const std::vector<VertexPN>& vertices,
                   const MaterialID& material);

TriangleMeshID
AddStaticTriangles(const std::vector<VertexPN>& vertices,
                   const std::vector<unsigned>& indices,
                   const MaterialID& material);

// Vertex Position3 + Color3 + Normal3

void
Triangle(const VertexPCN& v1, const VertexPCN& v2, const VertexPCN& v3);

void
Triangles(const std::vector<VertexPCN>& vertices);

void
Triangles(const std::vector<VertexPCN>& vertices,
          const std::vector<unsigned>& indices);

TriangleMeshID
AddStaticTriangles(const std::vector<VertexPCN>& vertices);

TriangleMeshID
AddStaticTriangles(const std::vector<VertexPCN>& vertices,
                   const std::vector<unsigned>& indices);

// General

void
DeleteStaticTriangles(const TriangleMeshID& meshID);

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------