#pragma once

#include "Core/ClassDefs.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "RenderingNamespaceDefs.h"
#include "ShaderBinding.h"
#include "VertexData.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class BufferPrivate
{
public:
  STATIC_CLASS(BufferPrivate)

  // VertexData
  static void Bind(const VertexDataInstance& data);
  static unsigned GetNVertices(const VertexDataInstance& data);
  static unsigned GetLayoutSize(const VertexDataInstance& data);

  // IndexData
  static void Bind(const IndexDataInstance& data);
  static unsigned GetNIndices(const IndexDataInstance& data);

  // InstancedDataBuffer
  static void Bind(const InstancedDataInstance& buffer);
  static unsigned GetNInstances(const InstancedDataInstance& buffer);
  static unsigned GetInstanceSize(const InstancedDataInstance& buffer);

  // ShaderBinding
  static const VertexDataInstance& GetVertexData(const ShaderBinding& binding);
  static const InstancedDataInstance& GetInstancedData(
    const ShaderBinding& binding);
  static unsigned GetLocation(const ShaderBinding& binding);
  static unsigned GetNEntries(const ShaderBinding& binding);
  static unsigned GetDataType(const ShaderBinding& binding);
  static unsigned GetOffset(const ShaderBinding& binding);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------