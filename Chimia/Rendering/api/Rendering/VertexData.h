#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "Rendering/DataLayout.h"
#include "Rendering/DataListeners.h"
#include "Rendering/IDataChangeListener.h"
#include "RenderingNamespaceDefs.h"

#include <memory>

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class VertexData
{
public:
  NON_COPYABLE(VertexData)

  static std::shared_ptr<VertexData> New();

  VertexData(VertexData&& other);
  VertexData& operator=(VertexData&& other);

  ~VertexData();

  void Create(const RawDataView& vertexData, const DataLayout& dataLayout);
  void Load(const RawDataView& data);
  void Resize(const RawDataView& data);

  void Clear();

private:
  VertexData() = default;

  friend class BufferPrivate;

  const DataLayout& GetDataLayout() const;
  unsigned GetNVertices() const;
  unsigned GetLayoutSize() const;
  void Bind() const;
  void AddListener(IDataChangeListener* listener);
  void RemoveListener(IDataChangeListener* listener);

  void AllocateVertexData(const RawDataView& vertexData,
                          const unsigned nVertices);

  unsigned m_VBO = 0;

  DataLayout m_dataLayout;
  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;

  size_t m_currentVertexSize = 0;
  size_t m_maximumVertexSize = 0;

  DataListeners m_listeners;
};

// ----------------------------------------------------------------------------

using VertexDataInstance = std::shared_ptr<VertexData>;

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------