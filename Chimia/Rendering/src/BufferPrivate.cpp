#include "BufferPrivate.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

const VertexRenderData&
BufferPrivate::GetBaseBuffer(const VertexBuffer& buffer)
{
  return buffer.m_buffer;
}

// --------------------------------------------------------------------------------------

const VertexRenderData&
BufferPrivate::GetBaseBuffer(const IndexedVertexBuffer& buffer)
{
  return buffer.m_buffer;
}

// --------------------------------------------------------------------------------------
// GenericVertexBuffer
// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const VertexRenderData& reusableVertexBuffer)
{
  reusableVertexBuffer.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const VertexRenderData& reusableVertexBuffer)
{
  return reusableVertexBuffer.GetNVertices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNElements(const VertexRenderData& reusableVertexBuffer)
{
  return reusableVertexBuffer.GetNIndices();
}

// --------------------------------------------------------------------------------------

bool
BufferPrivate::HasIndices(const VertexRenderData& reusableVertexBuffer)
{
  return reusableVertexBuffer.HasIndices();
}

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