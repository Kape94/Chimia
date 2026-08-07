#include "DataLayout.h"

#include "DataLayoutUtils.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

DataLayout::DataLayout(const std::initializer_list<DataSpec>& dataSpecs)
{
  size_t currentOffset = 0;
  for (const DataSpec& spec : dataSpecs) {
    m_dataItems.push_back({ spec, currentOffset });

    const size_t size = DataLayoutUtils::DataTypeSize(spec.type);
    currentOffset += size;
  }
}

// ----------------------------------------------------------------------------

size_t
DataLayout::OffsetOf(const std::string& name) const
{
  for (const DataItem& data : m_dataItems) {
    if (data.spec.name == name) {
      return data.offset;
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------

eDataType
DataLayout::DataTypeOf(const std::string& name) const
{
  for (const DataItem& data : m_dataItems) {
    if (data.spec.name == name) {
      return data.spec.type;
    }
  }
  return eDataType::FLOAT;
}

// ----------------------------------------------------------------------------

size_t
DataLayout::TotalSize() const
{
  size_t total = 0;
  for (const DataItem& data : m_dataItems) {
    total += DataLayoutUtils::DataTypeSize(data.spec.type);
  }
  return total;
}

// ----------------------------------------------------------------------------

void
DataLayout::ForEachSpec(std::function<void(const DataSpec&)> handleSpec) const
{
  for (const DataItem& data : m_dataItems) {
    handleSpec(data.spec);
  }
}

// ----------------------------------------------------------------------------