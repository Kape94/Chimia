#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class GenericVertexBuffer
{
public:
  DEFAULT_CONSTUCTIBLE(GenericVertexBuffer)
  NON_COPYABLE(GenericVertexBuffer)

  GenericVertexBuffer(GenericVertexBuffer&& other);
  GenericVertexBuffer& operator=(GenericVertexBuffer&& other);

  ~GenericVertexBuffer();

  void Create(const RawDataView& vertexData, const unsigned nVertices);
  void Create(const RawDataView& vertexData,
              const unsigned nVertices,
              const RawArrayView& indexData);

  void LoadVertexData(const RawDataView& data);
  void LoadIndexData(const RawArrayView& indexData);

  void Clear();

private:
  void Bind() const;
  unsigned GetNVertices() const;
  unsigned GetNIndices() const;

  bool HasIndices() const;

  void AllocateVertexData(const RawDataView& vertexData,
                          const unsigned nVertices);
  void AllocateIndexData(const RawArrayView& indexData);

  friend class BufferPrivate;

  unsigned m_VBO = 0;
  unsigned m_EBO = 0;

  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;
  unsigned m_nIndices = 0;

  size_t m_currentVertexSize = 0;
  size_t m_maximumVertexSize = 0;
  size_t m_currentIndexSize = 0;
  size_t m_maximumIndexSize = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------