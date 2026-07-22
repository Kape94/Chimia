#include "BufferPrivate.h"
#include "ShaderBinding.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const VertexData& data)
{
  data.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const VertexData& data)
{
  return data.GetNVertices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetLayoutSize(const VertexData& data)
{
  return data.GetLayoutSize();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const IndexData& data)
{
  data.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNIndices(const IndexData& data)
{
  return data.GetNIndices();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const InstancedDataBuffer& buffer)
{
  buffer.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNInstances(const InstancedDataBuffer& buffer)
{
  return buffer.GetNInstances();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetInstanceSize(const InstancedDataBuffer& buffer)
{
  return buffer.GetInstanceSize();
}

// --------------------------------------------------------------------------------------

const VertexData*
BufferPrivate::GetVertexData(const ShaderBinding& binding)
{
  return binding.GetVertexData();
}

// --------------------------------------------------------------------------------------

const InstancedDataBuffer*
BufferPrivate::GetInstancedData(const ShaderBinding& binding)
{
  return binding.GetInstancedData();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetLocation(const ShaderBinding& binding)
{
  return binding.GetLocation();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNEntries(const ShaderBinding& binding)
{
  return binding.GetNEntries();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetDataType(const ShaderBinding& binding)
{
  return binding.GetDataType();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetOffset(const ShaderBinding& binding)
{
  return binding.GetOffset();
}

// --------------------------------------------------------------------------------------