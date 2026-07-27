#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "Rendering/IDataChangeListener.h"
#include "RenderingNamespaceDefs.h"

#include <set>

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class DataListeners
{
public:
  NON_COPYABLE(DataListeners)

  DEFAULT_CONSTUCTIBLE(DataListeners)

  DataListeners(DataListeners&&);
  DataListeners& operator=(DataListeners&&);

  void Add(IDataChangeListener* listener);
  void Remove(IDataChangeListener* listener);

  void DataChanged();

private:
  std::set<IDataChangeListener*> m_listeners;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------