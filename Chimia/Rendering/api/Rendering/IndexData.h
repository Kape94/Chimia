#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class IndexData
{
public:
  DEFAULT_CONSTUCTIBLE(IndexData)
  NON_COPYABLE(IndexData)

  IndexData(IndexData&& other);
  IndexData& operator=(IndexData&& other);

  ~IndexData();

  void Create(const RawArrayView& indexData);

  void LoadIndexData(const RawArrayView& indexData);

  void Clear();

private:
  void Bind() const;
  unsigned GetNIndices() const;

  void AllocateIndexData(const RawArrayView& indexData);

  friend class BufferPrivate;

  unsigned m_EBO = 0;
  unsigned m_nIndices = 0;

  size_t m_currentIndexSize = 0;
  size_t m_maximumIndexSize = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------