#include "Normal3.h"

#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
#include "IlluminationPrivate.h"
#include "Renderers.h"
#include "RenderersUtils.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE
USING_DEFAULT_RENDERERS_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

constexpr eVertexLayout VERTEX_LAYOUT = eVertexLayout::POSITION3_NORMAL3;

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resource)
{
  const MaterialID materialID = resource.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForRendering(shader, VERTEX_LAYOUT, resource);
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}

// ----------------------------------------------------------------------------

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resource)
{
  const MaterialID materialID = resource.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resource);
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Normal3::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(VERTEX_LAYOUT,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Normal3::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

void
Normal3::DrawTriangle(const glm::vec3& p1,
                      const glm::vec3& p1Normal,
                      const glm::vec3& p2,
                      const glm::vec3& p2Normal,
                      const glm::vec3& p3,
                      const glm::vec3& p3Normal,
                      const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1Normal, NORM3_SIZE },
      { &p2, POS3_SIZE },
      { &p2Normal, NORM3_SIZE },
      { &p3, POS3_SIZE },
      { &p3Normal, NORM3_SIZE },
    },
    resource);
}

// ----------------------------------------------------------------------------
