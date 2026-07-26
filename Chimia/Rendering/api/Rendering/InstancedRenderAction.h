#pragma once

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "GenericRenderAction.h"
#include "IndexData.h"
#include "Rendering/InstancedData.h"
#include "RenderingNamespaceDefs.h"
#include "ShaderAttribute.h"
#include "VertexData.h"

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

  void CreateInstanced(const VertexDataInstance& reusableVertexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const InstancedDataInstance& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const VertexDataInstance& reusableVertexBuffer,
                       const IndexDataInstance& reusableIndexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const InstancedDataInstance& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void RelinkInstancedData(const InstancedDataInstance& instancesData,
                           const ShaderAttributes& instanceShaderAttributes);

  void Clear();

  void Render() const;

private:
  GenericRenderAction m_action;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE
