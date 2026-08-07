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

class InstancedData
{
public:
  NON_COPYABLE(InstancedData)

  InstancedData(InstancedData&& other);
  InstancedData& operator=(InstancedData&& other);

  ~InstancedData();

  static std::shared_ptr<InstancedData> Create(const RawDataView& instancedData,
                                               const DataLayout& dataLayout);
  void Load(const RawArrayView& data);
  void Resize(const RawDataView& data);

private:
  InstancedData();

  friend class BufferPrivate;
  const DataLayout& GetDataLayout() const;
  void Bind() const;
  unsigned GetNInstances() const;
  unsigned GetInstanceSize() const;
  DataListeners& GetListeners();

  void Setup(const RawDataView& instancedData, const DataLayout& dataLayout);
  void Clear();

  unsigned m_instancedVBO = 0;
  unsigned m_nInstances = 0;

  DataLayout m_dataLayout;
  size_t m_currentSize = 0;
  size_t m_maximumSize = 0;

  std::unique_ptr<DataListeners> m_listeners;
};

// ----------------------------------------------------------------------------

using InstancedDataInstance = std::shared_ptr<InstancedData>;

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------