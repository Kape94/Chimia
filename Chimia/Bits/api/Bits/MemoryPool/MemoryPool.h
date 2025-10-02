#pragma once

#include <vector>

#include "Bits/Common/BitsNamespaceDefs.h"

//-----------------------------------------------------------------------------

BEGIN_BITS_NAMESPACE

//-----------------------------------------------------------------------------

template<class Object>
class MemoryPool
{
public:
  MemoryPool(const size_t initialPoolSize);

  ~MemoryPool();

  Object* New();

  template<typename... Args>
  Object* New(Args&&... args);

  void Release(Object* obj);

private:
  inline Object* RetrieveAvailable();

  inline size_t InstanceIndex(Object* obj) const;

  inline void DeleteInstance(Object* obj);

  size_t poolSize = 0;
  Object* poolData = nullptr;

  std::vector<Object*> inUse;
  std::vector<Object*> availables;
};

//-----------------------------------------------------------------------------
// Impl
//-----------------------------------------------------------------------------

template<class Object>
MemoryPool<Object>::MemoryPool(const size_t initialPoolSize)
{
  poolData =
    static_cast<Object*>(::operator new(initialPoolSize * sizeof(Object)));
  poolSize = initialPoolSize;

  availables.reserve(poolSize);
  for (int i = 0; i < poolSize; ++i) {
    availables.push_back(poolData + i);
  }

  inUse.resize(poolSize);
  for (int i = 0; i < poolSize; ++i) {
    inUse[i] = nullptr;
  }
}

//-----------------------------------------------------------------------------

template<class Object>
MemoryPool<Object>::~MemoryPool()
{
  for (Object* obj : inUse) {
    if (obj != nullptr) {
      DeleteInstance(obj);
    }
  }

  ::operator delete(poolData);
}

//-----------------------------------------------------------------------------

template<class Object>
Object*
MemoryPool<Object>::New()
{
  Object* newObj = RetrieveAvailable();
  new (newObj) Object();
  return newObj;
}

//-----------------------------------------------------------------------------

template<class Object>
template<typename... Args>
Object*
MemoryPool<Object>::New(Args&&... args)
{
  Object* newObj = RetrieveAvailable();
  new (newObj) Object(std::forward<Args>(args)...);
  return newObj;
}

//-----------------------------------------------------------------------------

template<class Object>
inline Object*
MemoryPool<Object>::RetrieveAvailable()
{
  Object* newObj = availables.back();
  availables.pop_back();

  const size_t i = InstanceIndex(newObj);
  inUse[i] = newObj;

  return newObj;
}

//-----------------------------------------------------------------------------

template<class Object>
void
MemoryPool<Object>::Release(Object* obj)
{
  DeleteInstance(obj);

  const size_t i = InstanceIndex(obj);
  inUse[i] = nullptr;

  availables.push_back(obj);
}

//-----------------------------------------------------------------------------

template<class Object>
inline size_t
MemoryPool<Object>::InstanceIndex(Object* obj) const
{
  return obj - poolData;
}

//-----------------------------------------------------------------------------

template<class Object>
inline void
MemoryPool<Object>::DeleteInstance(Object* obj)
{
  obj->~Object();
}

//-----------------------------------------------------------------------------

END_BITS_NAMESPACE

//-----------------------------------------------------------------------------
