#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "GenericVertexBuffer.h"
#include "RenderingNamespaceDefs.h"

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

  void LoadVertexData(const RawDataView& data);
  void LoadIndexData(const RawArrayView& indexData);

private:
  void Clear();

  const GenericVertexBuffer& GetBaseBuffer() const;

  friend class BufferPrivate;

  GenericVertexBuffer m_buffer;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------