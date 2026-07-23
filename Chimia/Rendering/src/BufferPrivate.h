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
  static void Bind(const VertexData& data);
  static unsigned GetNVertices(const VertexData& data);
  static unsigned GetLayoutSize(const VertexData& data);

  // IndexData
  static void Bind(const IndexData& data);
  static unsigned GetNIndices(const IndexData& data);

  // InstancedDataBuffer
  static void Bind(const InstancedData& buffer);
  static unsigned GetNInstances(const InstancedData& buffer);
  static unsigned GetInstanceSize(const InstancedData& buffer);

  // ShaderBinding
  static const VertexData* GetVertexData(const ShaderBinding& binding);
  static const InstancedData* GetInstancedData(const ShaderBinding& binding);
  static unsigned GetLocation(const ShaderBinding& binding);
  static unsigned GetNEntries(const ShaderBinding& binding);
  static unsigned GetDataType(const ShaderBinding& binding);
  static unsigned GetOffset(const ShaderBinding& binding);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------