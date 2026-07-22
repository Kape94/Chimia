/*

#include "PrimitiveRenderingData.h"

#include "Core/Types.h"
#include "IndexData.h"
#include <VertexData.h>

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

void
PrimitiveRenderingData::Create(const RawDataView& vertexData,
                               const unsigned nVertices)
{
  Clear();

  m_ownVertexData.reset(new VertexData);
  m_ownVertexData->Create(vertexData, nVertices);
}

// ----------------------------------------------------------------------------

void
PrimitiveRenderingData::Create(const RawDataView& vertexData,
                               const unsigned nVertices,
                               const RawArrayView& indexData)
{
  Clear();

  m_ownVertexData.reset(new VertexData);
  m_ownVertexData->Create(vertexData, nVertices);

  m_ownIndexData.reset(new IndexData);
  m_ownIndexData->Create(indexData);
}

// ----------------------------------------------------------------------------

void
PrimitiveRenderingData::Create(const VertexData& referenceVertexData)
{
  Clear();

  m_referenceVertexData = &referenceVertexData;
}

// ----------------------------------------------------------------------------

void
PrimitiveRenderingData::Create(const VertexData& referenceVertexData,
                               const RawArrayView& indexData)
{
  Clear();

  m_referenceVertexData = &referenceVertexData;

  m_ownIndexData.reset(new IndexData);
  m_ownIndexData->Create(indexData);
}

// ----------------------------------------------------------------------------

void
PrimitiveRenderingData::LoadVertexData(const RawDataView& data)
{
  if (m_ownVertexData == nullptr) {
    return;
  }

  m_ownVertexData->Load(data);
}

// ----------------------------------------------------------------------------

void
PrimitiveRenderingData::LoadIndexData(const RawArrayView& indexData)
{
  if (m_ownIndexData == nullptr) {
    return;
  }

  m_ownIndexData->LoadIndexData(indexData);
}

// ----------------------------------------------------------------------------

void
PrimitiveRenderingData::Clear()
{
  m_ownVertexData.reset(nullptr);
  m_referenceVertexData = nullptr;

  m_ownIndexData.reset(nullptr);
  m_referenceIndexData = nullptr;
}

// ----------------------------------------------------------------------------
*/