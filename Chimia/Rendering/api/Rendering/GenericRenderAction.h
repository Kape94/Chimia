#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

#include "GenericVertexBuffer.h"
#include "ShaderAttribute.h"

#include "Core/Types.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class GenericRenderAction
{
public:
  DEFAULT_CONSTUCTIBLE(GenericRenderAction)
  NON_COPYABLE(GenericRenderAction)

  GenericRenderAction(GenericRenderAction&& other) noexcept;
  GenericRenderAction& operator=(GenericRenderAction&& other) noexcept;

  GenericRenderAction(const RawDataView& vertexData,
                      const ShaderAttributes& shaderAttributes);

  ~GenericRenderAction();

  void Create(const GenericVertexBuffer& reusableVertexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const RawArrayView& indexData,
              const ShaderAttributes& shaderAttributes);

  void LoadVertexData(const RawDataView& vertexData);
  void LoadIndexData(const RawArrayView& indexData);

  void Clear();

  void Render() const;

private:
  size_t CalculateNumberOfVertices(
    const RawDataView& vertexData,
    const ShaderAttributes& shaderAttributes) const;

  void Configure(const GenericVertexBuffer& buffer,
                 const ShaderAttributes& shaderAttributes);

  friend class BufferPrivate;

  unsigned m_VAO = 0;

  std::unique_ptr<GenericVertexBuffer> m_ownBuffer = nullptr;
  const GenericVertexBuffer* m_referenceBuffer = nullptr;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------