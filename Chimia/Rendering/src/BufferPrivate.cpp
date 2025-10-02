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
