#include "TransitionRenderingComponent.h"

#include "Draw3DPrivate.h"
#include "ImmediateTransitionsBatch.h"
#include "ResourcesManager.h"

#include "Core/Diagnostics.h"
#include "Transition.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
TransitionRenderingComponent::Init(const BatchingSettings& batchingSettings,
                                   const DataBindingProvider& dataBindings,
                                   const std::function<void(void)>& onFlush)
{
  m_batchingSettings = batchingSettings;
  m_dataBindingProvider = dataBindings;
  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

void
TransitionRenderingComponent::Flush(
  const eImmediateFlusingPolicy flushingPolicy)
{
  m_transitionsTable.ForEach(
    [flushingPolicy](ImmediateTransitionsBatch& transitionsBatch) {
      transitionsBatch.Flush(flushingPolicy);
    });
}

// ----------------------------------------------------------------------------

void
TransitionRenderingComponent::DrawTransition(const TransitionID& transitionID,
                                             const RawDataView& instanceData)
{
  const unsigned idValue = Draw3DPrivate::GetTransitionIDValue(transitionID);
  ImmediateTransitionsBatch* batch = m_transitionsTable.Find(idValue);
  if (batch == nullptr) {
    batch = AllocateImmediateBatchForTransition(transitionID);
    if (batch == nullptr) {
      return;
    }
  }

  batch->Draw(instanceData);
}

// ----------------------------------------------------------------------------

void
TransitionRenderingComponent::DrawTransition(
  const TransitionID& transitionID,
  const std::initializer_list<RawDataView>& instanceDatas)
{
  const unsigned idValue = Draw3DPrivate::GetTransitionIDValue(transitionID);
  ImmediateTransitionsBatch* batch = m_transitionsTable.Find(idValue);
  if (batch == nullptr) {
    batch = AllocateImmediateBatchForTransition(transitionID);
    if (batch == nullptr) {
      return;
    }
  }

  batch->Draw(instanceDatas);
}

// ----------------------------------------------------------------------------

ImmediateTransitionsBatch*
TransitionRenderingComponent::AllocateImmediateBatchForTransition(
  const TransitionID& transitionID)
{
  const unsigned idValue = Draw3DPrivate::GetTransitionIDValue(transitionID);

  ImmediateTransitionsBatch* batch = m_transitionsTable.InsertWithID(idValue);
  if (batch == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at ModelRenderingComponent::DrawModel, couldn't "
      "create transformed model entry for new model created");
    return nullptr;
  }

  const Transition* transition =
    ResourcesManager::GetInstance().GetTransition(transitionID);
  batch->Create(
    *transition, m_batchingSettings, m_dataBindingProvider, m_onFlush);
  return batch;
}

// ----------------------------------------------------------------------------