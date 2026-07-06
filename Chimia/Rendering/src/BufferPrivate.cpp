#include "BufferPrivate.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetVAO(const RenderAction& buffer)
{
  return buffer.GetVAO();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const RenderAction& buffer)
{
  return buffer.GetNVertices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetVAO(const IndexedRenderAction& buffer)
{
  return buffer.GetVAO();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNElements(const IndexedRenderAction& buffer)
{
  return buffer.GetNElements();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const VertexBuffer& reusableVertexBuffer)
{
  reusableVertexBuffer.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const VertexBuffer& reusableVertexBuffer)
{
  return reusableVertexBuffer.GetNVertices();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const IndexedVertexBuffer& reusableBuffer)
{
  reusableBuffer.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNElements(const IndexedVertexBuffer& reusableBuffer)
{
  return reusableBuffer.GetNIndices();
}

// --------------------------------------------------------------------------------------

const VertexBuffer&
BufferPrivate::GetBaseVertexBuffer(const IndexedVertexBuffer& reusableBuffer)
{
  return reusableBuffer.GetBaseBuffer();
}

// --------------------------------------------------------------------------------------