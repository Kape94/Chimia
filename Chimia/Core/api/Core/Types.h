#pragma once

// ----------------------------------------------------------------------------

#include "CoreNamespaceDefs.h"

#include <cstddef>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

struct RawDataView
{
  const void* data;
  const size_t size;

  RawDataView(const void* _data, const size_t _size)
    : data(_data)
    , size(_size)
  {
  }

  template<typename T>
  RawDataView(const std::vector<T>& vector)
    : data(vector.data())
    , size(vector.size() * sizeof(T))
  {
  }

  template<typename T>
  const T& As()
  {
    return *reinterpret_cast<const T*>(data);
  }
};

// ----------------------------------------------------------------------------

struct RawArrayView
{
  const void* array;
  const size_t nItems;
  const size_t itemSize;

  size_t TotalSize() const { return nItems * itemSize; }
  RawDataView AsDataView() const { return { array, TotalSize() }; }

  RawArrayView(const void* _array, const size_t _nItems, const size_t _itemSize)
    : array(_array)
    , nItems(_nItems)
    , itemSize(_itemSize)
  {
  }

  template<typename T>
  RawArrayView(const std::vector<T>& vector)
    : array(vector.data())
    , nItems(vector.size())
    , itemSize(sizeof(T))
  {
  }

  template<typename T>
  RawArrayView(const T* rawArray, const size_t nItemsInArray)
    : array(rawArray)
    , nItems(nItemsInArray)
    , itemSize(sizeof(T))
  {
  }

  template<typename T>
  static RawArrayView FromVector(const std::vector<T>& vector)
  {
    return RawArrayView(vector);
  }
};

// ----------------------------------------------------------------------------

struct VertexDataView
{
  RawDataView dataView;
  size_t nVertices;
};

// ----------------------------------------------------------------------------

struct MeshAttributes
{
  bool hasVertexColor = false;
  bool hasVertexNormal = false;
  bool hasTexCoords = false;

  // User can request colors without alpha
  bool includeColorAlpha = true;
};

struct MeshBufferData
{
  std::vector<float> vertexDataValues;
  std::vector<unsigned> indices;
  MeshAttributes attributes;
  size_t nVertices = 0;

  bool HasIndices() const { return indices.size() > 0; }
};

class Mesh
{
public:
  void AddPosition(const glm::vec3& pos);
  void AddColor(const glm::vec4& col);
  void AddNormal(const glm::vec3& normal);
  void AddTexCoord(const glm::vec2& texCoord);
  void AddIndex(const unsigned i);

  MeshAttributes GetMeshAttributes() const;
  const MeshBufferData& GetRenderData() const;

private:
  void BuildRenderData() const;

  mutable bool m_shouldRebuildRenderData = true;
  mutable MeshBufferData m_renderData;

  std::vector<glm::vec3> m_positions;
  std::vector<glm::vec4> m_colors;
  std::vector<glm::vec3> m_normals;
  std::vector<glm::vec2> m_texCoords;
  std::vector<unsigned> m_indices;
};

// ----------------------------------------------------------------------------

END_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------