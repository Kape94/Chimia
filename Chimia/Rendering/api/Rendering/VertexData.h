#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "Rendering/DataLayout.h"
#include "RenderingNamespaceDefs.h"

#include <memory>

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class DataListeners;

// ----------------------------------------------------------------------------

class VertexData
{
public:
  NON_COPYABLE(VertexData)

  VertexData(VertexData&& other);
  VertexData& operator=(VertexData&& other);

  ~VertexData();

  static std::shared_ptr<VertexData> Create(const RawDataView& vertexData,
                                            const DataLayout& dataLayout);

  void Load(const RawDataView& data);
  void Resize(const RawDataView& data);

private:
  VertexData();

  friend class BufferPrivate;
  const DataLayout& GetDataLayout() const;
  unsigned GetNVertices() const;
  unsigned GetLayoutSize() const;
  void Bind() const;
  DataListeners& GetListeners();

  void Clear();
  void Setup(const RawDataView& vertexData, const DataLayout& dataLayout);
  void AllocateVertexData(const RawDataView& vertexData,
                          const unsigned nVertices);

  unsigned m_VBO = 0;

  DataLayout m_dataLayout;
  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;

  size_t m_currentVertexSize = 0;
  size_t m_maximumVertexSize = 0;

  std::unique_ptr<DataListeners> m_listeners;
};

// ----------------------------------------------------------------------------

using VertexDataInstance = std::shared_ptr<VertexData>;

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------