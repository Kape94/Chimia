#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"
#include "ReusableVertexBufferObject.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class ReusableIndexedVertexBufferObject
{
public:
  DEFAULT_CONSTUCTIBLE(ReusableIndexedVertexBufferObject)
  NON_COPYABLE(ReusableIndexedVertexBufferObject)

  ReusableIndexedVertexBufferObject(ReusableIndexedVertexBufferObject&& other);
  ReusableIndexedVertexBufferObject& operator=(
    ReusableIndexedVertexBufferObject&& other);

  ~ReusableIndexedVertexBufferObject();

  void Create(const RawDataView& vertexData,
              const unsigned nVertices,
              const RawArrayView& indexData);

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