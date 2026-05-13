#pragma once

//---------------------------------------------------------------------------------------

#include "RenderingNamespaceDefs.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

void
Initialize();

void
SetViewport(const unsigned x,
            const unsigned y,
            const unsigned width,
            const unsigned height);

void
Clear(const float r = 0.0f,
      const float g = 0.0f,
      const float b = 0.0f,
      const float a = 0.0f);

void
EnableDepthTest(const bool enable);

void
EnableDepthMask(const bool enable);

void
EnableColorBlend(const bool enable);

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
