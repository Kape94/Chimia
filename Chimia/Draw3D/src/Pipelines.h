#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "Pipeline.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Pipelines
{
public:
  STATIC_CLASS(Pipelines)

  static void Init();

  static void ActivateRegularPipeline();
  static void ActivateTransparentRenderingPipeline();

  static const Pipeline& CurrentPipeline();
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------