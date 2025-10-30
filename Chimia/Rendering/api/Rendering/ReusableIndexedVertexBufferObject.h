#pragma once

// ----------------------------------------------------------------------------

#include "RenderingNamespaceDefs.h"
#include "ReusableVertexBufferObject.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class ReusableIndexedVertexBufferObject
{
public:
  ReusableIndexedVertexBufferObject() = default;

  ReusableIndexedVertexBufferObject(
    const ReusableIndexedVertexBufferObject& other) = delete;
  ReusableIndexedVertexBufferObject& operator=(
    const ReusableIndexedVertexBufferObject& other) = delete;

  ReusableIndexedVertexBufferObject(ReusableIndexedVertexBufferObject&& other);
  ReusableIndexedVertexBufferObject& operator=(
    ReusableIndexedVertexBufferObject&& other);

  ~ReusableIndexedVertexBufferObject();

  void Create(const void* vertexData,
              const unsigned vertexDataSize,
              const unsigned nVertices,
              const unsigned* indexData,
              const unsigned nIndexDataItems);

private:
  void Clear();

  void Bind() const;
  unsigned GetNVertices() const;
  unsigned GetNIndices() const;
  const ReusableVertexBufferObject& GetBaseBuffer() const;

  friend class BufferPrivate;

  ReusableVertexBufferObject m_vertexBufferObject;
  unsigned m_EBO = 0;
  unsigned m_nIndices = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------