#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class ReusableVertexBufferObject
{
public:
  DEFAULT_CONSTUCTIBLE(ReusableVertexBufferObject)
  NON_COPYABLE(ReusableVertexBufferObject)

  ReusableVertexBufferObject(ReusableVertexBufferObject&& other);
  ReusableVertexBufferObject& operator=(ReusableVertexBufferObject&& other);

  ~ReusableVertexBufferObject();

  void Create(const RawDataView& vertexData, const unsigned nVertices);

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