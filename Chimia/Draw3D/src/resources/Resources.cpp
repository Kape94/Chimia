#include "Resources.h"

#include "ResourcesManager.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

ModelID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateModel(const MeshDataView& meshData,
                                          const eVertexLayout vertexLayout)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateModel(meshData, vertexLayout);
}

// ----------------------------------------------------------------------------

MaterialID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateMaterial(const glm::vec3& ambient,
                                             const glm::vec3& diffuse,
                                             const glm::vec3& specular,
                                             const float shininess)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateMaterial(ambient, diffuse, specular, shininess);
}

// ----------------------------------------------------------------------------

TextureID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateTexture(const unsigned char* textureData,
                                            const unsigned width,
                                            const unsigned height)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateTexture(textureData, width, height);
}

// ----------------------------------------------------------------------------

OpacityFactorID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateOpacityFactor(const float value)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateOpacityFactor(value);
}

// ----------------------------------------------------------------------------
