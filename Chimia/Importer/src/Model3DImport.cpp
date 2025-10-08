#include "Model3DImport.h"

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

// ----------------------------------------------------------------------------

USING_CHIMIA_IMPORTER_NAMESPACE

// ----------------------------------------------------------------------------

std::vector<Mesh>
Chimia::Importer::ImportMeshes(const std::string& path)
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
    if (mesh->HasPositions()) {
      for (size_t j = 0; j < mesh->mNumVertices; ++j) {
        const aiVector3D& v = mesh->mVertices[j];
        importedMesh.vertices.emplace_back(vector3f{ v.x, v.y, v.z });
      }
    }

    if (mesh->HasVertexColors(0)) {
      for (size_t j = 0; j < mesh->mNumVertices; ++j) {
        const aiColor4D& c = mesh->mColors[0][j];
        importedMesh.colors.emplace_back(vector4f{ c.r, c.g, c.b, c.a });
      }
    }

    if (mesh->HasNormals()) {
      for (size_t j = 0; j < mesh->mNumVertices; ++j) {
        const aiVector3D& n = mesh->mNormals[j];
        importedMesh.normals.emplace_back(vector3f{ n.x, n.y, n.z });
      }
    }

    if (mesh->HasFaces()) {
      for (size_t j = 0; j < mesh->mNumFaces; ++j) {
        const aiFace& face = mesh->mFaces[j];

        for (size_t k = 0; k < face.mNumIndices; ++k) {
          importedMesh.indices.push_back(face.mIndices[k]);
        }
      }
    }
  }

  return meshes;
}

// ----------------------------------------------------------------------------