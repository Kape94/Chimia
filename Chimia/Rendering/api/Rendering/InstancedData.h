#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class InstancedData
{
public:
  DEFAULT_CONSTUCTIBLE(InstancedData)
  NON_COPYABLE(InstancedData)

  InstancedData(InstancedData&& other);
  InstancedData& operator=(InstancedData&& other);

  ~InstancedData();

  void Create(const RawArrayView& instancedData);
  void Load(const RawArrayView& data);

  void Clear();

private:
  friend class BufferPrivate;

  void Bind() const;
  unsigned GetNInstances() const;
  unsigned GetInstanceSize() const;

  unsigned m_instancedVBO = 0;
  unsigned m_nInstances = 0;

  size_t m_currentSize = 0;
  size_t m_maximumSize = 0;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------