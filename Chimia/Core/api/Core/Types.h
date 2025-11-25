#pragma once

// ----------------------------------------------------------------------------

#include "CoreNamespaceDefs.h"

#include <cstddef>
#include <vector>

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

class Position3
{
public:
  Position3() = delete;
  explicit Position3(float _x, float _y, float _z)
    : x(_x)
    , y(_y)
    , z(_z)
  {
  }

  float x = 0.0f, y = 0.0f, z = 0.0f;
};

class Color4
{
public:
  Color4() = delete;
  explicit Color4(float _r, float _g, float _b, float _a)
    : r(_r)
    , g(_g)
    , b(_b)
    , a(_a)
  {
  }

  float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
};

class Normal3
{
public:
  Normal3() = delete;
  explicit Normal3(float _x, float _y, float _z)
    : x(_x)
    , y(_y)
    , z(_z)
  {
  }

  float x = 0.0f, y = 0.0f, z = 0.0f;
};

class TexCoord2
{
public:
  TexCoord2() = delete;
  explicit TexCoord2(float _u, float _v)
    : u(_u)
    , v(_v)
  {
  }

  float u = 0.0f, v = 0.0f;
};

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

  bool HasIndices() const { return indices.size() > 0; }
};

class Mesh
{
public:
  void Add(const Position3& pos);
  void Add(const Color4& col);
  void Add(const Normal3& normal);
  void Add(const TexCoord2& texCoord);

  MeshAttributes GetMeshAttributes() const;
  MeshBufferData GeneratePackedBufferData(
    const struct MeshAttributes& selectedAttributes) const;

private:
  std::vector<Position3> m_positions;
  std::vector<Color4> m_colors;
  std::vector<Normal3> m_normals;
  std::vector<TexCoord2> m_texCoords;
  std::vector<unsigned> m_indices;
};

// ----------------------------------------------------------------------------

END_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------