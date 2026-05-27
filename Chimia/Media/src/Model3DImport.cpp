#include "Model3DImport.h"
#include "MediaTypes.h"

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

// ----------------------------------------------------------------------------

USING_CHIMIA_MEDIA_NAMESPACE

// ----------------------------------------------------------------------------

namespace Model3DImportInternal {

void
ExtractMeshData(const aiMesh& mesh, Mesh& output);
void
ExtractMeshPositions(const aiMesh& mesh, Mesh& output);
void
ExtractMeshColors(const aiMesh& mesh, Mesh& output);
void
ExtractMeshNormals(const aiMesh& mesh, Mesh& output);
void
ExtractMeshIndices(const aiMesh& mesh, Mesh& output);

void
FillBufferFromMesh(const Mesh& mesh,
                   const MeshAttributesToInclude& attributes,
                   BufferData& data);
}

// ----------------------------------------------------------------------------
// Importer
// ----------------------------------------------------------------------------

std::vector<Mesh>
Chimia::Media::ImportMeshes(const std::string& path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
    path,
    aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
      aiProcess_JoinIdenticalVertices);

  if (!scene) {
    printf(
      "Model (%s) failed to load: %s", path.c_str(), importer.GetErrorString());
    return {};
  }

  std::vector<Mesh> meshes;
  for (size_t i = 0; i < scene->mNumMeshes; ++i) {
    const aiMesh* mesh = scene->mMeshes[i];

    Mesh& importedMesh = meshes.emplace_back();
    Model3DImportInternal::ExtractMeshData(*mesh, importedMesh);
  }

  return meshes;
}

// ----------------------------------------------------------------------------

std::vector<BufferData>
Chimia::Media::PackBufferDataFromMeshes(
  const std::vector<Mesh>& meshes,
  const MeshAttributesToInclude& attributes)
{
  std::vector<BufferData> bufferDatas;
  bufferDatas.reserve(meshes.size());

  for (const Mesh& mesh : meshes) {
    BufferData& data = bufferDatas.emplace_back();
    Model3DImportInternal::FillBufferFromMesh(mesh, attributes, data);
  }

  return bufferDatas;
}

// ----------------------------------------------------------------------------
// Model3DImportInternal
// ----------------------------------------------------------------------------

void
Model3DImportInternal::ExtractMeshData(const aiMesh& mesh, Mesh& output)
{
  ExtractMeshPositions(mesh, output);
  ExtractMeshColors(mesh, output);
  ExtractMeshNormals(mesh, output);
  ExtractMeshIndices(mesh, output);
}

// ----------------------------------------------------------------------------

void
Model3DImportInternal::ExtractMeshPositions(const aiMesh& mesh, Mesh& output)
{
  if (mesh.HasPositions()) {
    for (size_t j = 0; j < mesh.mNumVertices; ++j) {
      const aiVector3D& v = mesh.mVertices[j];
      output.vertices.emplace_back(Vector3f{ v.x, v.y, v.z });
    }
  }
}

// ----------------------------------------------------------------------------

void
Model3DImportInternal::ExtractMeshColors(const aiMesh& mesh, Mesh& output)
{
  if (mesh.HasVertexColors(0)) {
    for (size_t j = 0; j < mesh.mNumVertices; ++j) {
      const aiColor4D& c = mesh.mColors[0][j];
      output.colors.emplace_back(Color4f{ c.r, c.g, c.b, c.a });
    }
  }
}

// ----------------------------------------------------------------------------

void
Model3DImportInternal::ExtractMeshNormals(const aiMesh& mesh, Mesh& output)
{
  if (mesh.HasNormals()) {
    for (size_t j = 0; j < mesh.mNumVertices; ++j) {
      const aiVector3D& n = mesh.mNormals[j];
      output.normals.emplace_back(Vector3f{ n.x, n.y, n.z });
    }
  }
}

// ----------------------------------------------------------------------------

void
Model3DImportInternal::ExtractMeshIndices(const aiMesh& mesh, Mesh& output)
{
  if (mesh.HasFaces()) {
    for (size_t j = 0; j < mesh.mNumFaces; ++j) {
      const aiFace& face = mesh.mFaces[j];

      for (size_t k = 0; k < face.mNumIndices; ++k) {
        output.indices.push_back(face.mIndices[k]);
      }
    }
  }
}

// ----------------------------------------------------------------------------

void
Model3DImportInternal::FillBufferFromMesh(
  const Mesh& mesh,
  const MeshAttributesToInclude& attributes,
  BufferData& data)
{
  const size_t nVertices = mesh.vertices.size();

  const bool hasColors =
    attributes.includeColors && mesh.colors.size() == nVertices;
  const bool hasNormals =
    attributes.includeNormals && mesh.normals.size() == nVertices;
  const bool hasTexCoords =
    attributes.includeTexCoords && mesh.texCoords.size() == nVertices;

  for (size_t i = 0; i < nVertices; ++i) {
    const Vector3f& pos = mesh.vertices[i];
    data.vertexData.insert(data.vertexData.end(), { pos.x, pos.y, pos.z });

    if (hasColors) {
      const Color4f& col = mesh.colors[i];
      data.vertexData.insert(data.vertexData.end(), { col.r, col.g, col.b });
    }
    if (hasNormals) {
      const Vector3f& n = mesh.normals[i];
      data.vertexData.insert(data.vertexData.end(), { n.x, n.y, n.z });
    }
    if (hasTexCoords) {
      const Vector2f& uv = mesh.texCoords[i];
      data.vertexData.insert(data.vertexData.end(), { uv.x, uv.y });
    }
  }

  data.indices = mesh.indices;
}

// ----------------------------------------------------------------------------