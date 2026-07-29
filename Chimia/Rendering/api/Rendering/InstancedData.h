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

class InstancedData
{
public:
  NON_COPYABLE(InstancedData)

  static std::shared_ptr<InstancedData> New();

  InstancedData(InstancedData&& other);
  InstancedData& operator=(InstancedData&& other);

  ~InstancedData();

  void Create(const RawDataView& instancedData, const DataLayout& dataLayout);
  void Load(const RawArrayView& data);
  void Resize(const RawDataView& data);

  void Clear();

private:
  InstancedData() = default;

  friend class BufferPrivate;

  const DataLayout& GetDataLayout() const;
  void Bind() const;
  unsigned GetNInstances() const;
  unsigned GetInstanceSize() const;
  void AddListener(IDataChangeListener* listener);
  void RemoveListener(IDataChangeListener* listener);

  unsigned m_instancedVBO = 0;
  unsigned m_nInstances = 0;

  DataLayout m_dataLayout;
  size_t m_currentSize = 0;
  size_t m_maximumSize = 0;

  DataListeners m_listeners;
};

// ----------------------------------------------------------------------------

using InstancedDataInstance = std::shared_ptr<InstancedData>;

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------