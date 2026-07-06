#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

#include "IndexedVertexBuffer.h"
#include "RenderAction.h"
#include "ShaderAttribute.h"

#include "Core/Types.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class IndexedRenderAction
{
public:
  DEFAULT_CONSTUCTIBLE(IndexedRenderAction)

  NON_COPYABLE(IndexedRenderAction)

  IndexedRenderAction(IndexedRenderAction&& other) noexcept;
  IndexedRenderAction& operator=(IndexedRenderAction&& other) noexcept;

  IndexedRenderAction(const RawDataView& vertexData,
                      const RawArrayView& indexData,
                      const ShaderAttributes& shaderAttributes);

  ~IndexedRenderAction();

  void Create(const IndexedVertexBuffer& reusableVertexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const RawArrayView& indexData,
              const ShaderAttributes& shaderAttributes);

  void LoadVertexData(const RawDataView& newVertexData);
  void LoadIndexData(const RawArrayView& newIndexData);

  void Clear();

  void Render() const;

private:
  void LoadIndexDataInGPU(const unsigned* indexData,
                          const unsigned nIndexDataItems);

  unsigned GetVAO() const;

  unsigned GetNElements() const;

  friend class BufferPrivate;

  RenderAction m_baseBuffer;
  unsigned m_EBO = 0;

  unsigned m_nElements = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------