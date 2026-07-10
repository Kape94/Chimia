#pragma once

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "GenericRenderAction.h"
#include "RenderingNamespaceDefs.h"

#include "IndexedVertexBuffer.h"
#include "ShaderAttribute.h"
#include "VertexBuffer.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class InstancedRenderAction
{
public:
  DEFAULT_CONSTUCTIBLE(InstancedRenderAction)
  NON_COPYABLE(InstancedRenderAction)

  InstancedRenderAction(InstancedRenderAction&& other) noexcept;
  InstancedRenderAction& operator=(InstancedRenderAction&& other) noexcept;

  ~InstancedRenderAction();

  void CreateInstanced(const VertexBuffer& reusableVertexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const IndexedVertexBuffer& reusableVertexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const RawDataView& vertexData,
                       const RawArrayView& indexData,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const RawDataView& vertexData,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void LoadInstancedData(const RawArrayView& instancesData);

  void RecreateInstancedBuffer(
    const RawArrayView& instancesData,
    const ShaderAttributes& instanceShaderAttributes);

  void Clear();

  void Render() const;

private:
  GenericRenderAction m_action;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE
