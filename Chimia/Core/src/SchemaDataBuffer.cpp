#include "SchemaDataBuffer.h"

#include <cassert>
#include <cstring>
#include <numeric>

// ----------------------------------------------------------------------------

USING_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------
// Utils
// ----------------------------------------------------------------------------

namespace {
size_t
ComputeTotalSchemaSize(const SchemaDataBuffer::DataSchema& schema)
{
  return std::accumulate(
    schema.begin(),
    schema.end(),
    0,
    [](const size_t current, const size_t next) { return current + next; });
}

std::vector<size_t>
GenerateDataOffsets(const SchemaDataBuffer::DataSchema& schema)
{
  std::vector<size_t> offsets;
  offsets.reserve(schema.size());

  size_t currentOffset = 0;
  for (size_t itemSize : schema) {
    offsets.push_back(currentOffset);
    currentOffset += itemSize;
  }

  return offsets;
}
}

// ----------------------------------------------------------------------------
// SchemaDataBuffer
// ----------------------------------------------------------------------------

SchemaDataBuffer::SchemaDataBuffer(const DataSchema& schema)
  : m_buffer(ComputeTotalSchemaSize(schema))
  , m_schema(schema)
  , m_offsets(GenerateDataOffsets(schema))
{
}

// ----------------------------------------------------------------------------

void
SchemaDataBuffer::Set(const size_t index, const RawDataView& data)
{
  const size_t schemaItemSize = m_schema[index];
  assert(data.size == schemaItemSize);

  unsigned char* rawData = m_buffer.GetData();
  const size_t itemOffset = m_offsets[index];

  unsigned char* itemData = rawData + itemOffset;
  memcpy(itemData, data.data, data.size);
}

// ----------------------------------------------------------------------------

RawDataView
SchemaDataBuffer::Get(const size_t index) const
{
  const unsigned char* rawData = m_buffer.GetData();
  const size_t itemOffset = m_offsets[index];

  const unsigned char* itemData = rawData + itemOffset;
  const size_t itemSize = m_schema[index];

  return { itemData, itemSize };
}

// ----------------------------------------------------------------------------

size_t
SchemaDataBuffer::NItems() const
{
  return m_offsets.size();
}

// ----------------------------------------------------------------------------