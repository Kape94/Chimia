#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

#include "IndexedBuffer.h"
#include "ReusableIndexedVertexBufferObject.h"
#include "ReusableVertexBufferObject.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class BufferPrivate
{
public:
  STATIC_CLASS(BufferPrivate)

  // Buffer
  static unsigned GetVAO(const RenderAction& buffer);
  static unsigned GetNVertices(const RenderAction& buffer);

  // IndexedBuffer
  static unsigned GetVAO(const IndexedRenderAction& buffer);
  static unsigned GetNElements(const IndexedRenderAction& buffer);

  // ReusableVertexBufferObject
  static void Bind(const ReusableVertexBufferObject& reusableVertexBuffer);
  static unsigned GetNVertices(
    const ReusableVertexBufferObject& reusableVertexBuffer);
  static unsigned GetNElements(
    const ReusableVertexBufferObject& reusableVertexBuffer);

  // ReusableIndexedVertexBufferObject
  static void Bind(const ReusableIndexedVertexBufferObject& reusableBuffer);
  static unsigned GetNElements(
    const ReusableIndexedVertexBufferObject& reusableBuffer);
  static const ReusableVertexBufferObject& GetBaseVertexBuffer(
    const ReusableIndexedVertexBufferObject& reusableBuffer);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------