#pragma once

// ----------------------------------------------------------------------------

#include "CoreNamespaceDefs.h"
#include "DataBuffer.h"
#include "Types.h"

#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

class SchemaDataBuffer
{
public:
  using DataSchema = std::vector<size_t>;

  SchemaDataBuffer() = delete;

  SchemaDataBuffer(const DataSchema& schema);

  SchemaDataBuffer(const SchemaDataBuffer&) = delete;
  SchemaDataBuffer& operator=(const SchemaDataBuffer&) = delete;

  void Set(const size_t index, const RawDataView& data);
  RawDataView Get(const size_t index) const;
  size_t NItems() const;

private:
  DataSchema m_schema;
  std::vector<size_t> m_offsets;
  DataBuffer m_buffer;
};

// ----------------------------------------------------------------------------

END_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------