#include "DataListeners.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

DataListeners::DataListeners(DataListeners&& other)
  : m_listeners(other.m_listeners)
{
}

// ----------------------------------------------------------------------------

DataListeners&
DataListeners::operator=(DataListeners&& other)
{
  m_listeners = std::move(other.m_listeners);
  return *this;
}

// ----------------------------------------------------------------------------

void
DataListeners::Add(IDataChangeListener* listener)
{
  m_listeners.insert(listener);
}

// ----------------------------------------------------------------------------

void
DataListeners::Remove(IDataChangeListener* listener)
{
  m_listeners.erase(listener);
}

// ----------------------------------------------------------------------------

void
DataListeners::DataChanged()
{
  /*During the notification handling, a listener can unsubscribe itself. In
   * order to prevent iteration invalidation, we create a clone of the
   * container
   */
  const std::set<IDataChangeListener*> listenersClone(m_listeners);
  for (IDataChangeListener* listener : listenersClone) {
    listener->DataChanged();
  }
}

// ----------------------------------------------------------------------------