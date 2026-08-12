#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"
#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Transition
{
public:
  NON_COPYABLE_NON_MOVABLE(Transition)

  Transition(const ModelID& start, const ModelID& target);

  eVertexLayout GetVertexLayout() const;

  const ModelID& GetStart() const;
  const ModelID& GetTarget() const;

private:
  eVertexLayout m_vertexLayout;

  const ModelID m_start;
  const ModelID m_target;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------