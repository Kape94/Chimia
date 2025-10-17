#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <functional>
#include <map>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

template<class Object>
class IdentifiedObjectTable
{
public:
  IdentifiedObjectTable() = default;

  std::pair<unsigned, Object*> Insert();

  Object* Find(unsigned id);

  void ForEach(const std::function<void(Object&)>& action);

private:
  IdentifiedObjectTable(const IdentifiedObjectTable& other) = delete;
  IdentifiedObjectTable& operator=(const IdentifiedObjectTable& other) = delete;

  IdentifiedObjectTable(IdentifiedObjectTable&& other) = delete;
  IdentifiedObjectTable& operator=(IdentifiedObjectTable&& other) = delete;

  unsigned m_currentObjectID = 1;
  std::map<unsigned, Object> m_objectsTable;
};

// ----------------------------------------------------------------------------

template<class Object>
std::pair<unsigned, Object*>
IdentifiedObjectTable<Object>::Insert()
{
  unsigned modelID = m_currentObjectID++;

  auto it = m_objectsTable.emplace(
    std::make_pair<unsigned, Object>(std::move(modelID), {}));
  const bool insertedWithSuccess = it.second;
  if (!insertedWithSuccess) {
    return { 0, nullptr };
  }

  return { modelID, &it.first->second };
}

// ----------------------------------------------------------------------------

template<class Object>
Object*
IdentifiedObjectTable<Object>::Find(unsigned id)
{
  auto it = m_objectsTable.find(id);
  return it != m_objectsTable.end() ? &it->second : nullptr;
}

// ----------------------------------------------------------------------------

template<class Object>
void
IdentifiedObjectTable<Object>::ForEach(
  const std::function<void(Object&)>& action)
{
  for (auto& tableEntry : m_objectsTable) {
    action(tableEntry.second);
  }
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------