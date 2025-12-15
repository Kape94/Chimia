#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "Core/Types.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"
#include "eRendererType.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

const ResourceGroupID EMPTY_RESOURCE =
  ResourcesManager::GetInstance().CreateResourceGroup();

void
ConfigureForTriangleDrawing(const ResourcesGroup& resource)
{
  Chimia::Rendering::Shader& shader = Shaders::VertexColored();
  shader.Use();
  CameraPrivate::SetCameraOnShader(shader);
}

void
ConfigureForInstancedDrawing(const ResourcesGroup& resource)
{
  Chimia::Rendering::Shader& shader =
    Shaders::VertexColoredWithInstancedTransform();
  shader.Use();
  CameraPrivate::SetCameraOnShader(shader);
}

}

// ----------------------------------------------------------------------------

VertexColoredRendererImpl&
VertexColoredRendererImpl::getInstance()
{
  static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3)
  };

  static const Chimia::Rendering::ShaderAttributes
    TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
      Chimia::Rendering::ShaderAttribute::Float(2 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4)
    };

  constexpr unsigned RENDERER_ID =
    static_cast<unsigned>(eRendererType::VERTEX_COLORED);

  static VertexColoredRendererImpl renderer(
    RENDERER_ID,
    VERTEX_ATTRIBUTES,
    TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
    ConfigureForTriangleDrawing,
    ConfigureForInstancedDrawing);
  return renderer;
}

// ----------------------------------------------------------------------------

VertexColoredRendererImpl::VertexColoredRendererImpl(
  const unsigned id,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instancedAttributes,
  void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  void (*setupShaderForInstancedRendering)(const ResourcesGroup&))
  : m_renderer(id,
               vertexAttributes,
               instancedAttributes,
               setupShaderForTriangleRendering,
               setupShaderForInstancedRendering)
{
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::Init()
{
  m_renderer.Init();
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawTriangle(const glm::vec3& p1,
                                        const glm::vec3& color1,
                                        const glm::vec3& p2,
                                        const glm::vec3& color2,
                                        const glm::vec3& p3,
                                        const glm::vec3& color3)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COL3_SIZE = sizeof(glm::vec3);

  m_renderer.DrawTriangle({ { &p1, POS3_SIZE },
                            { &color1, COL3_SIZE },
                            { &p2, POS3_SIZE },
                            { &color2, COL3_SIZE },
                            { &p3, POS3_SIZE },
                            { &color3, COL3_SIZE } },
                          EMPTY_RESOURCE);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawTriangles(const RawArrayView& vertexDataArray)
{
  m_renderer.DrawTriangles(vertexDataArray, EMPTY_RESOURCE);
}

// ----------------------------------------------------------------------------

TriangleMeshID
VertexColoredRendererImpl::AddStaticTriangles(const RawDataView& vertexData)
{
  return m_renderer.AddStaticTriangles(vertexData, EMPTY_RESOURCE);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  m_renderer.DeleteStaticTriangles(meshID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawModelTransformed(const ModelID& modelID,
                                                const glm::mat4x4& transform)
{
  m_renderer.DrawModelTransformed(modelID, transform, EMPTY_RESOURCE);
}

// ----------------------------------------------------------------------------

ModelInstanceID
VertexColoredRendererImpl::AddStaticModel(const ModelID& modelID,
                                          const glm::mat4x4& transform)
{
  return m_renderer.AddStaticModel(modelID, transform, EMPTY_RESOURCE);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  m_renderer.DeleteStaticModel(instanceID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::Flush()
{
  m_renderer.Flush();
}

// ----------------------------------------------------------------------------
