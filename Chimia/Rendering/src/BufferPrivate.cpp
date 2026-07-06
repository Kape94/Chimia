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
BufferPrivate::Bind(const ReusableVertexBufferObject& reusableVertexBuffer)
{
  reusableVertexBuffer.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(
  const ReusableVertexBufferObject& reusableVertexBuffer)
{
  return reusableVertexBuffer.GetNVertices();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const ReusableIndexedVertexBufferObject& reusableBuffer)
{
  reusableBuffer.Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNElements(
  const ReusableIndexedVertexBufferObject& reusableBuffer)
{
  return reusableBuffer.GetNIndices();
}

// --------------------------------------------------------------------------------------

const ReusableVertexBufferObject&
BufferPrivate::GetBaseVertexBuffer(
  const ReusableIndexedVertexBufferObject& reusableBuffer)
{
  return reusableBuffer.GetBaseBuffer();
}

// --------------------------------------------------------------------------------------