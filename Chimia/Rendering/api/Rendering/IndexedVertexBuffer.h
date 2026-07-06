#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"
#include "VertexBuffer.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class IndexedVertexBuffer
{
public:
  DEFAULT_CONSTUCTIBLE(IndexedVertexBuffer)
  NON_COPYABLE(IndexedVertexBuffer)

  IndexedVertexBuffer(IndexedVertexBuffer&& other);
  IndexedVertexBuffer& operator=(IndexedVertexBuffer&& other);

  ~IndexedVertexBuffer();

  void Create(const RawDataView& vertexData,
              const unsigned nVertices,
              const RawArrayView& indexData);

private:
  void Clear();

  void Bind() const;
  unsigned GetNVertices() const;
  unsigned GetNIndices() const;
  const VertexBuffer& GetBaseBuffer() const;

  friend class BufferPrivate;

  VertexBuffer m_vertexBufferObject;
  unsigned m_EBO = 0;
  unsigned m_nIndices = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------