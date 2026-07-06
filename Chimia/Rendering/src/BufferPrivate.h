#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

#include "IndexedRenderAction.h"
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
  static void Bind(const VertexBuffer& reusableVertexBuffer);
  static unsigned GetNVertices(const VertexBuffer& reusableVertexBuffer);
  static unsigned GetNElements(const VertexBuffer& reusableVertexBuffer);

  // ReusableIndexedVertexBufferObject
  static void Bind(const IndexedVertexBuffer& reusableBuffer);
  static unsigned GetNElements(const IndexedVertexBuffer& reusableBuffer);
  static const VertexBuffer& GetBaseVertexBuffer(
    const IndexedVertexBuffer& reusableBuffer);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------