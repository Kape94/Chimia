#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class VertexData
{
public:
  DEFAULT_CONSTUCTIBLE(VertexData)
  NON_COPYABLE(VertexData)

  VertexData(VertexData&& other);
  VertexData& operator=(VertexData&& other);

  ~VertexData();

  void Create(const RawDataView& vertexData, const unsigned nVertices);
  void Load(const RawDataView& data);

  void Clear();

private:
  friend class BufferPrivate;

  unsigned GetNVertices() const;
  void Bind() const;

  void AllocateVertexData(const RawDataView& vertexData,
                          const unsigned nVertices);

  unsigned m_VBO = 0;

  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;

  size_t m_currentVertexSize = 0;
  size_t m_maximumVertexSize = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------