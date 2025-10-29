#include "BufferPrivate.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetVAO(const Buffer& buffer)
{
  return buffer.GetVAO();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const Buffer& buffer)
{
  return buffer.GetNVertices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetVAO(const IndexedBuffer& buffer)
{
  return buffer.GetVAO();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNElements(const IndexedBuffer& buffer)
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

const ShaderAttributes&
BufferPrivate::GetShaderAttributes(
  const ReusableVertexBufferObject& reusableVertexBuffer)
{
  return reusableVertexBuffer.GetShaderAttributes();
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

const ShaderAttributes&
BufferPrivate::GetShaderAttributes(
  const ReusableIndexedVertexBufferObject& reusableBuffer)
{
  return reusableBuffer.GetShaderAttributes();
}

// --------------------------------------------------------------------------------------

const ReusableVertexBufferObject&
BufferPrivate::GetBaseVertexBuffer(
  const ReusableIndexedVertexBufferObject& reusableBuffer)
{
  return reusableBuffer.GetBaseBuffer();
}

// --------------------------------------------------------------------------------------