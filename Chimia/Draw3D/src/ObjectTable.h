#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

template<class Object>
class ObjectTable
{
public:
  ObjectTable();
  ~ObjectTable();

  ObjectTable(const ObjectTable&) = delete;
  ObjectTable& operator=(const ObjectTable&) = delete;

  std::pair<unsigned, Object*> Insert();
  Object* Insert(unsigned id);

  Object* Find(unsigned id);
  const Object* Find(unsigned id) const;

  void ForEach(const std::function<void(Object&)>& action);

  void Delete(unsigned id);

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
ObjectTable<Object>::ObjectTable()
  : m_objects(INITIAL_SIZE)
{
}

// ----------------------------------------------------------------------------

template<class Object>
ObjectTable<Object>::~ObjectTable()
{
  for (Object* obj : m_objects) {
    DeleteObject(obj);
  }
  m_objects.clear();
}

// ----------------------------------------------------------------------------

template<class Object>
std::pair<unsigned, Object*>
ObjectTable<Object>::Insert()
{
  int index = FindAvailableIndex();
  if (index == NOT_FOUND_INDEX) {
    m_objects.resize(m_objects.size() + GROWTH_SIZE);
    int index = FindAvailableIndex();
  }

  Object* newObject = new Object();
  m_objects[index] = newObject;

  return { static_cast<unsigned>(index), newObject };
}

// ----------------------------------------------------------------------------

template<class Object>
Object*
ObjectTable<Object>::Insert(unsigned id)
{
  if (id > m_objects.size()) {
    m_objects.resize(id + 1);
  }

  if (m_objects[id] == nullptr) {
    Object* newObject = new Object();
    m_objects[id] = newObject;
    return newObject;
  }
  return nullptr;
}

// ----------------------------------------------------------------------------

template<class Object>
Object*
ObjectTable<Object>::Find(unsigned id)
{
  if (id > m_objects.size()) {
    return nullptr;
  }

  return m_objects[id];
}

// ----------------------------------------------------------------------------

template<class Object>
const Object*
ObjectTable<Object>::Find(unsigned id) const
{
  if (id > m_objects.size()) {
    return nullptr;
  }

  return m_objects[id];
}

// ----------------------------------------------------------------------------

template<class Object>
void
ObjectTable<Object>::ForEach(const std::function<void(Object&)>& action)
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
ObjectTable<Object>::Delete(unsigned id)
{
  if (id > m_objects.size()) {
    return;
  }

  DeleteObject(m_objects[id]);
}

// ----------------------------------------------------------------------------

template<class Object>
int
ObjectTable<Object>::FindAvailableIndex() const
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
ObjectTable<Object>::DeleteObject(Object*& object)
{
  if (object != nullptr) {
    delete object;
    object = nullptr;
  }
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------