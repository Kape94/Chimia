#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class GLState
{
public:
  STATIC_CLASS(GLState)

  static void BindVertexArray(const unsigned id);
  static void BindArrayBuffer(const unsigned id);
  static void BindElementBuffer(const unsigned id);

  static void BindFramebuffer(const unsigned id);
  static void BindRenderbuffer(const unsigned id);

  static void BindTexture2D(const unsigned id);

  static void BindShader(const unsigned id);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------