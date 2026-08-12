#include "Transition.h"
#include "ResourcesManager.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

Transition::Transition(const ModelID& start, const ModelID& target)
  : m_start(start)
  , m_target(target)
{
  const eVertexLayout startLayout =
    ResourcesManager::GetInstance().GetModel(start)->GetVertexLayout();
  const eVertexLayout targetLayout =
    ResourcesManager::GetInstance().GetModel(target)->GetVertexLayout();

  assert(startLayout == targetLayout &&
         "Transition: start and target doesn't have same layout");
  m_vertexLayout = startLayout;
}

// ----------------------------------------------------------------------------

eVertexLayout
Transition::GetVertexLayout() const
{
  return m_vertexLayout;
}

// ----------------------------------------------------------------------------

const ModelID&
Transition::GetStart() const
{
  return m_start;
}

// ----------------------------------------------------------------------------

const ModelID&
Transition::GetTarget() const
{
  return m_target;
}

// ----------------------------------------------------------------------------