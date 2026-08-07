#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

#include <memory>

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class DataListeners;

// ----------------------------------------------------------------------------

class IndexData
{
public:
  NON_COPYABLE(IndexData)

  IndexData(IndexData&& other);
  IndexData& operator=(IndexData&& other);

  ~IndexData();

  static std::shared_ptr<IndexData> Create(const RawArrayView& indexData);

  void LoadIndexData(const RawArrayView& indexData);
  void Resize(const RawArrayView& indexData);

private:
  IndexData();

  friend class BufferPrivate;
  void Bind() const;
  unsigned GetNIndices() const;
  DataListeners& GetListeners();

  void AllocateIndexData(const RawArrayView& indexData);
  void Clear();

  unsigned m_EBO = 0;
  unsigned m_nIndices = 0;

  size_t m_currentIndexSize = 0;
  size_t m_maximumIndexSize = 0;

  std::unique_ptr<DataListeners> m_listeners;
};

// ----------------------------------------------------------------------------

using IndexDataInstance = std::shared_ptr<IndexData>;

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------