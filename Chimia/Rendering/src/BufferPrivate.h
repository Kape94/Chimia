#pragma once

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