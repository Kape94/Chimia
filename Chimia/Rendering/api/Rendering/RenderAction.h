#pragma once

#include "Core/ClassDefs.h"
#include "GenericRenderAction.h"
#include "RenderingNamespaceDefs.h"

#include "ShaderAttribute.h"
#include "VertexBuffer.h"

#include "Core/Types.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class RenderAction
{
public:
  DEFAULT_CONSTUCTIBLE(RenderAction)
  NON_COPYABLE(RenderAction)

  RenderAction(RenderAction&& other) noexcept;
  RenderAction& operator=(RenderAction&& other) noexcept;

  RenderAction(const RawDataView& vertexData,
               const ShaderAttributes& shaderAttributes);

  ~RenderAction();

  void Create(const VertexBuffer& reusableVertexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const ShaderAttributes& shaderAttributes);

  void Load(const RawDataView& vertexData);

  void Clear();

  void Render() const;

private:
  GenericRenderAction m_action;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------