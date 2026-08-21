#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "CoreNamespaceDefs.h"

#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

template<class Object>
class ObjectRegistry
{
public:
  ObjectRegistry();
  ~ObjectRegistry();

  NON_COPYABLE_NON_MOVABLE(ObjectRegistry)

  template<typename... Args>
  std::pair<unsigned, Object*> Insert(Args&&... args);

  template<typename... Args>
  Object* InsertWithID(unsigned id, Args&&... args);

  Object* Find(unsigned id);
  const Object* Find(unsigned id) const;

  void ForEach(const std::function<void(Object&)>& action);

  void Delete(unsigned id);
  void Delete(Object* obj);

private:
  // TODO: put these constants in the config
  static constexpr int NOT_FOUND_INDEX = -1;
  static constexpr size_t INITIAL_SIZE = 100;
  static constexpr size_t GROWTH_SIZE = 100;

  int FindAvailableIndex() const;
  void DeleteObject(Object*& object);

  std::vector<Object*> m_objects;
};

// ----------------------------------------------------------------------------

template<class Object>
ObjectRegistry<Object>::ObjectRegistry()
  : m_objects(INITIAL_SIZE)
{
}

// ----------------------------------------------------------------------------

template<class Object>
ObjectRegistry<Object>::~ObjectRegistry()
{
  for (Object* obj : m_objects) {
    DeleteObject(obj);
  }
  m_objects.clear();
}

// ----------------------------------------------------------------------------

template<class Object>
template<typename... Args>
std::pair<unsigned, Object*>
ObjectRegistry<Object>::Insert(Args&&... args)
{
  int index = FindAvailableIndex();
  if (index == NOT_FOUND_INDEX) {
    m_objects.resize(m_objects.size() + GROWTH_SIZE);
    index = FindAvailableIndex();
  }

  Object* newObject = new Object(std::forward<Args>(args)...);
  m_objects[index] = newObject;

  return { static_cast<unsigned>(index), newObject };
}

// ----------------------------------------------------------------------------

template<class Object>
template<typename... Args>
Object*
ObjectRegistry<Object>::InsertWithID(unsigned id, Args&&... args)
{
  if (id > m_objects.size()) {
    m_objects.resize(id + 1);
  }

  if (m_objects[id] == nullptr) {
    Object* newObject = new Object(std::forward<Args>(args)...);
    m_objects[id] = newObject;
    return newObject;
  }
  return nullptr;
}

// ----------------------------------------------------------------------------

template<class Object>
Object*
ObjectRegistry<Object>::Find(unsigned id)
{
  if (id > m_objects.size()) {
    return nullptr;
  }

  return m_objects[id];
}

// ----------------------------------------------------------------------------

template<class Object>
const Object*
ObjectRegistry<Object>::Find(unsigned id) const
{
  if (id > m_objects.size()) {
    return nullptr;
  }

  return m_objects[id];
}

// ----------------------------------------------------------------------------

template<class Object>
void
ObjectRegistry<Object>::ForEach(const std::function<void(Object&)>& action)
{
  for (Object* obj : m_objects) {
    if (obj != nullptr) {
      action(*obj);
    }
  }
}

// ----------------------------------------------------------------------------

template<class Object>
void
ObjectRegistry<Object>::Delete(unsigned id)
{
  if (id > m_objects.size()) {
    return;
  }

  DeleteObject(m_objects[id]);
}

// ----------------------------------------------------------------------------

template<class Object>
void
ObjectRegistry<Object>::Delete(Object* obj)
{
  for (unsigned i = 0; i < m_objects.size(); ++i) {
    if (m_objects[i] == obj) {
      Delete(i);
    }
  }
}

// ----------------------------------------------------------------------------

template<class Object>
int
ObjectRegistry<Object>::FindAvailableIndex() const
{
  for (unsigned i = 0; i < m_objects.size(); ++i) {
    if (m_objects[i] == nullptr)
      return i;
  }
  return NOT_FOUND_INDEX;
}

// ----------------------------------------------------------------------------

template<class Object>
void
ObjectRegistry<Object>::DeleteObject(Object*& object)
{
  if (object != nullptr) {
    delete object;
    object = nullptr;
  }
}

// ----------------------------------------------------------------------------

END_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------