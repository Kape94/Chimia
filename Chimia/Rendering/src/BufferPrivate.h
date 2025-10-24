#pragma once

#include "RenderingNamespaceDefs.h"

#include "IndexedBuffer.h"
#include "ReusableVertexBufferObject.h"
#include "ShaderAttribute.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class BufferPrivate
{
public:
  // Buffer
  static unsigned GetVAO(const Buffer& buffer);
  static unsigned GetNVertices(const Buffer& buffer);

  // IndexedBuffer
  static unsigned GetVAO(const IndexedBuffer& buffer);
  static unsigned GetNElements(const IndexedBuffer& buffer);

  // ReusableVertexBufferObject
  static void Bind(const ReusableVertexBufferObject& reusableVertexBuffer);
  static unsigned GetNVertices(
    const ReusableVertexBufferObject& reusableVertexBuffer);
  static unsigned GetNElements(
    const ReusableVertexBufferObject& reusableVertexBuffer);
  static const ShaderAttributes& GetShaderAttributes(
    const ReusableVertexBufferObject& reusableVertexBuffer);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------