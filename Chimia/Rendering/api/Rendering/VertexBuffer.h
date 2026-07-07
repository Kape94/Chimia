#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class VertexBuffer
{
public:
  DEFAULT_CONSTUCTIBLE(VertexBuffer)
  NON_COPYABLE(VertexBuffer)

  VertexBuffer(VertexBuffer&& other);
  VertexBuffer& operator=(VertexBuffer&& other);

  ~VertexBuffer();

  void Create(const RawDataView& vertexData, const unsigned nVertices);
  void Load(const RawDataView& data);

  void Clear();

private:
  void Bind() const;
  unsigned GetNVertices() const;

  friend class BufferPrivate;

  unsigned m_VBO = 0;
  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;

  size_t m_currentSize = 0;
  size_t m_maximumSize = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------