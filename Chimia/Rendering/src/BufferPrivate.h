#pragma once

#include "Core/ClassDefs.h"
#include "InstancedDataBuffer.h"
#include "RenderingNamespaceDefs.h"

#include "IndexedVertexBuffer.h"
#include "VertexBuffer.h"
#include "VertexData.h"
#include "VertexRenderData.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class BufferPrivate
{
public:
  STATIC_CLASS(BufferPrivate)

  // VertexBuffer
  static const VertexRenderData& GetBaseBuffer(const VertexBuffer& buffer);

  // IndexedVertexBuffer
  static const VertexRenderData& GetBaseBuffer(
    const IndexedVertexBuffer& buffer);

  // GenericVertexBuffer
  static void Bind(const VertexRenderData& reusableVertexBuffer);
  static unsigned GetNVertices(const VertexRenderData& reusableVertexBuffer);
  static unsigned GetNElements(const VertexRenderData& reusableVertexBuffer);
  static bool HasIndices(const VertexRenderData& reusableVertexBuffer);

  // VertexData
  static void Bind(const VertexData& data);
  static unsigned GetNVertices(const VertexData& data);

  // IndexData
  static void Bind(const IndexData& data);
  static unsigned GetNIndices(const IndexData& data);

  // InstancedDataBuffer
  static void Bind(const InstancedDataBuffer& buffer);
  static unsigned GetNInstances(const InstancedDataBuffer& buffer);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------