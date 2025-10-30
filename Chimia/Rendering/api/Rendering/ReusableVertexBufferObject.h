#pragma once

// ----------------------------------------------------------------------------

#include "RenderingNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class ReusableVertexBufferObject
{
public:
  ReusableVertexBufferObject() = default;

  ReusableVertexBufferObject(const ReusableVertexBufferObject& other) = delete;
  ReusableVertexBufferObject& operator=(
    const ReusableVertexBufferObject& other) = delete;

  ReusableVertexBufferObject(ReusableVertexBufferObject&& other);
  ReusableVertexBufferObject& operator=(ReusableVertexBufferObject&& other);

  ~ReusableVertexBufferObject();

  void Create(const void* vertexData,
              const unsigned vertexDataSize,
              const unsigned nVertices);

  void Clear();

private:
  void Bind() const;
  unsigned GetNVertices() const;

  friend class BufferPrivate;

  unsigned m_VBO = 0;
  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------