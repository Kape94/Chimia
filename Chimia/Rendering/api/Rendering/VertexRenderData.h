#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "IndexData.h"
#include "RenderingNamespaceDefs.h"
#include "VertexData.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class VertexRenderData
{
public:
  DEFAULT_CONSTUCTIBLE(VertexRenderData)
  NON_COPYABLE(VertexRenderData)

  VertexRenderData(VertexRenderData&& other);
  VertexRenderData& operator=(VertexRenderData&& other);

  ~VertexRenderData();

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

  VertexData m_vertexData;
  std::unique_ptr<IndexData> m_indexData;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------