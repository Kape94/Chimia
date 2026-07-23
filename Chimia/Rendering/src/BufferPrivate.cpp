#include "BufferPrivate.h"
#include "ShaderBinding.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const VertexDataInstance& data)
{
  data->Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const VertexDataInstance& data)
{
  return data->GetNVertices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetLayoutSize(const VertexDataInstance& data)
{
  return data->GetLayoutSize();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const IndexDataInstance& data)
{
  data->Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNIndices(const IndexDataInstance& data)
{
  return data->GetNIndices();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const InstancedDataInstance& buffer)
{
  buffer->Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNInstances(const InstancedDataInstance& buffer)
{
  return buffer->GetNInstances();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetInstanceSize(const InstancedDataInstance& buffer)
{
  return buffer->GetInstanceSize();
}

// --------------------------------------------------------------------------------------

const VertexDataInstance&
BufferPrivate::GetVertexData(const ShaderBinding& binding)
{
  return binding.GetVertexData();
}

// --------------------------------------------------------------------------------------

const InstancedDataInstance&
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