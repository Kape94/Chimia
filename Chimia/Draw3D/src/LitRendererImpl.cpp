#include "LitRendererImpl.h"

#include "Config.h"
#include "Draw3DPrivate.h"
#include "IlluminationPrivate.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "TriangleMeshComponent.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
  Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(1 /*normal*/, 3)
};

static const Chimia::Rendering::ShaderAttributes
  TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(2 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(6 /*materialAmbient*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(7 /*materialDiffuse*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(8 /*materialSpecular*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(9 /*materialShininess*/, 1)
  };

}

// ----------------------------------------------------------------------------

LitRendererImpl&
LitRendererImpl::getInstance()
{
  static LitRendererImpl renderer;
  return renderer;
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::Init()
{
  m_modelComponent.Init(Config::GouraudLit::modelsBatchSize,
                        VERTEX_ATTRIBUTES,
                        TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                        [&]() { ConfigureShaderForTransformedModelDrawing(); });
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DrawTriangle(const glm::vec3& p1,
                              const glm::vec3& normal1,
                              const glm::vec3& p2,
                              const glm::vec3& normal2,
                              const glm::vec3& p3,
                              const glm::vec3& normal3,
                              const MaterialID& materialID)
{
  constexpr size_t VEC3_SIZE = sizeof(glm::vec3);

  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  renderComponent->DrawTriangle({
    { &p1, VEC3_SIZE },
    { &normal1, VEC3_SIZE },
    { &p2, VEC3_SIZE },
    { &normal2, VEC3_SIZE },
    { &p3, VEC3_SIZE },
    { &normal3, VEC3_SIZE },
  });
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DrawTriangles(const std::vector<float>& vertexData,
                               const MaterialID& materialID)
{
  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  renderComponent->DrawTriangles(vertexData);
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DrawTriangles(const std::vector<float>& vertexData,
                               const std::vector<unsigned>& indexData,
                               const MaterialID& materialID)
{
  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  renderComponent->DrawTriangles(vertexData, indexData);
}

// ----------------------------------------------------------------------------

LitTriangleMeshID
LitRendererImpl::AddStaticTriangles(const std::vector<float>& vertexData,
                                    const MaterialID& materialID)
{
  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  const TriangleMeshID meshID = renderComponent->AddStaticMesh(vertexData);

  return Draw3DPrivate::CreateLitTriangleMeshID(
    Draw3DPrivate::GetTriangleMeshIDValue(meshID),
    Draw3DPrivate::GetMaterialIDValue(materialID));
}

// ----------------------------------------------------------------------------

LitTriangleMeshID
LitRendererImpl::AddStaticTriangles(const std::vector<float>& vertexData,
                                    const std::vector<unsigned>& indexData,
                                    const MaterialID& materialID)
{
  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  const TriangleMeshID meshID =
    renderComponent->AddStaticMesh(vertexData, indexData);

  return Draw3DPrivate::CreateLitTriangleMeshID(
    Draw3DPrivate::GetTriangleMeshIDValue(meshID),
    Draw3DPrivate::GetMaterialIDValue(materialID));
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DeleteStaticTriangles(const LitTriangleMeshID& meshID)
{
  auto [meshIDValue, materialIDValue] =
    Draw3DPrivate::GetLitTriangleMeshIDValues(meshID);

  auto renderComponent = FetchTriangleRenderComponentForMaterial(
    Draw3DPrivate::CreateMaterialID(materialIDValue));
  renderComponent->DeleteStaticMesh(
    Draw3DPrivate::CreateTriangleMeshID(meshIDValue));
}

// ----------------------------------------------------------------------------

TriangleMeshComponent*
LitRendererImpl::FetchTriangleRenderComponentForMaterial(
  const MaterialID& materialID)
{
  const unsigned idValue = Draw3DPrivate::GetMaterialIDValue(materialID);
  auto renderComponent = m_triangleMeshComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_triangleMeshComponents.Insert(idValue);

    renderComponent->Init(
      Config::GouraudLit::triangleBatchSizePerMaterial,
      VERTEX_ATTRIBUTES,
      [&]() { ConfigureShaderForTriangleDrawing(materialID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DrawModelTransformed(const ModelID& modelID,
                                      const glm::mat4x4& transform,
                                      const MaterialID& materialID)
{
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  m_modelComponent.DrawModel(modelID,
                             { { &transform, sizeof(glm::mat4x4) },
                               { &material->ambient, sizeof(glm::vec3) },
                               { &material->diffuse, sizeof(glm::vec3) },
                               { &material->specular, sizeof(glm::vec3) },
                               { &material->shininess, sizeof(float) } });
}

// ----------------------------------------------------------------------------

ModelInstanceID
LitRendererImpl::AddStaticModel(const ModelID& modelID,
                                const glm::mat4x4& transform,
                                const MaterialID& materialID)
{
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return Draw3DPrivate::CreateModelInstanceID(0, 0);
  }

  return m_modelComponent.AddStaticModel(
    modelID,
    { { &transform, sizeof(glm::mat4x4) },
      { &material->ambient, sizeof(glm::vec3) },
      { &material->diffuse, sizeof(glm::vec3) },
      { &material->specular, sizeof(glm::vec3) },
      { &material->shininess, sizeof(float) } });
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  m_modelComponent.DeleteStaticModel(instanceID);
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::Flush()
{
  m_triangleMeshComponents.ForEach(
    [](TriangleMeshComponent& triangleRenderer) { triangleRenderer.Flush(); });
  m_modelComponent.Flush();
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::ConfigureShaderForTriangleDrawing(const MaterialID& materialID)
{
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Chimia::Draw3D::Shaders::GouraudLit();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::ConfigureShaderForTransformedModelDrawing()
{
  Chimia::Rendering::Shader& shader =
    Chimia::Draw3D::Shaders::GouraudLitWithInstancedTransformAndMaterial();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

// ----------------------------------------------------------------------------
