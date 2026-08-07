#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

#include <functional>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

enum class eDataType
{
  FLOAT,
  DOUBLE,
  INT,
  UNSIGNED_INT,
  BOOL,
  VECTOR_2_FLOAT,
  VECTOR_3_FLOAT,
  VECTOR_4_FLOAT,
  MATRIX_FLOAT_4X4
};

// ----------------------------------------------------------------------------

class DataLayout
{
public:
  struct DataSpec
  {
    std::string name;
    eDataType type;
  };

  DEFAULT_CONSTUCTIBLE(DataLayout)
  DEFAULT_DESTRUCTIBLE(DataLayout)
  DEFAULT_COPYABLE(DataLayout)
  DEFAULT_MOVABLE(DataLayout)

  DataLayout(const std::initializer_list<DataSpec>& dataSpecs);

  size_t OffsetOf(const std::string& name) const;
  eDataType DataTypeOf(const std::string& name) const;
  size_t TotalSize() const;

  void ForEachSpec(std::function<void(const DataSpec&)>) const;

private:
  struct DataItem
  {
    DataSpec spec;
    size_t offset;
  };

  std::vector<DataItem> m_dataItems;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------