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

END_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------