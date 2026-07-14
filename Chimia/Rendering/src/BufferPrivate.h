#pragma once

#include "Core/ClassDefs.h"
#include "InstancedDataBuffer.h"
#include "RenderingNamespaceDefs.h"

#include "GenericVertexBuffer.h"
#include "IndexedVertexBuffer.h"
#include "VertexBuffer.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class BufferPrivate
{
public:
  STATIC_CLASS(BufferPrivate)

  // VertexBuffer
  static const GenericVertexBuffer& GetBaseBuffer(const VertexBuffer& buffer);

  // IndexedVertexBuffer
  static const GenericVertexBuffer& GetBaseBuffer(
    const IndexedVertexBuffer& buffer);

  // GenericVertexBuffer
  static void Bind(const GenericVertexBuffer& reusableVertexBuffer);
  static unsigned GetNVertices(const GenericVertexBuffer& reusableVertexBuffer);
  static unsigned GetNElements(const GenericVertexBuffer& reusableVertexBuffer);
  static bool HasIndices(const GenericVertexBuffer& reusableVertexBuffer);

  // InstancedDataBuffer
  static void Bind(const InstancedDataBuffer& buffer);
  static unsigned GetNInstances(const InstancedDataBuffer& buffer);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------