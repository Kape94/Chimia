#pragma once

// ----------------------------------------------------------------------------

/*
#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "IndexData.h"
#include "RenderingNamespaceDefs.h"
#include "VertexData.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

enum class ePrimitive
{
  POINT,
  LINE,
  TRIANGLE
};

// ----------------------------------------------------------------------------

class PrimitiveRenderingData
{
public:
  DEFAULT_CONSTUCTIBLE(PrimitiveRenderingData)
  NON_COPYABLE(PrimitiveRenderingData)

  PrimitiveRenderingData(PrimitiveRenderingData&& other);
  PrimitiveRenderingData& operator=(PrimitiveRenderingData&& other);

  ~PrimitiveRenderingData();

  void Create(const RawDataView& vertexData,
              const unsigned nVertices,
              const ePrimitive primitive);

  void Create(const RawDataView& vertexData,
              const unsigned nVertices,
              const RawArrayView& indexData,
              const ePrimitive primitive);

  void Create(const VertexData& referenceVertexData,
              const ePrimitive primitive);

  void Create(const VertexData& referenceVertexData,
              const RawArrayView& indexData,
              const ePrimitive primitive);

  void LoadVertexData(const RawDataView& data);
  void LoadIndexData(const RawArrayView& indexData);

  void Clear();

private:
  friend class BufferPrivate;

  unsigned GetNVertices() const;
  void Bind() const;

  ePrimitive m_primitive;

  std::unique_ptr<VertexData> m_ownVertexData = nullptr;
  const VertexData* m_referenceVertexData = nullptr;

  std::unique_ptr<IndexData> m_ownIndexData = nullptr;
  const VertexData* m_referenceIndexData = nullptr;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE
*/

// ----------------------------------------------------------------------------