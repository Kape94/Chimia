#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

#include "Buffer.h"
#include "ReusableIndexedVertexBufferObject.h"
#include "ShaderAttribute.h"

#include "Core/Types.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class IndexedBuffer
{
public:
  DEFAULT_CONSTUCTIBLE(IndexedBuffer)

  NON_COPYABLE(IndexedBuffer)

  IndexedBuffer(IndexedBuffer&& other) noexcept;
  IndexedBuffer& operator=(IndexedBuffer&& other) noexcept;

  IndexedBuffer(const RawDataView& vertexData,
                const RawArrayView& indexData,
                const ShaderAttributes& shaderAttributes);

  ~IndexedBuffer();

  void Create(const ReusableIndexedVertexBufferObject& reusableVertexBuffer,
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

  Buffer m_baseBuffer;
  unsigned m_EBO = 0;

  unsigned m_nElements = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------