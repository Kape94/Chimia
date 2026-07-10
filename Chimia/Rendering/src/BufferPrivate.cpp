#include "BufferPrivate.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

const GenericVertexBuffer&
BufferPrivate::GetBaseBuffer(const VertexBuffer& buffer)
{
  return buffer.m_buffer;
}

// --------------------------------------------------------------------------------------

const GenericVertexBuffer&
BufferPrivate::GetBaseBuffer(const IndexedVertexBuffer& buffer)
{
  return buffer.m_buffer;
}

// --------------------------------------------------------------------------------------
// GenericVertexBuffer
// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const GenericVertexBuffer& reusableVertexBuffer)
{
  reusableVertexBuffer.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const GenericVertexBuffer& reusableVertexBuffer)
{
  return reusableVertexBuffer.GetNVertices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNElements(const GenericVertexBuffer& reusableVertexBuffer)
{
  return reusableVertexBuffer.GetNIndices();
}

// --------------------------------------------------------------------------------------

bool
BufferPrivate::HasIndices(const GenericVertexBuffer& reusableVertexBuffer)
{
  return reusableVertexBuffer.HasIndices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNInstances(const InstancedDataBuffer& buffer)
{
  return buffer.GetNInstances();
}

// --------------------------------------------------------------------------------------