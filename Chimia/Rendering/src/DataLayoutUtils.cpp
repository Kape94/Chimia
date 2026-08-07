#include "DataLayoutUtils.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

size_t
DataLayoutUtils::DataTypeSize(const eDataType type)
{
  switch (type) {
    case eDataType::FLOAT:
      return sizeof(float);
    case eDataType::DOUBLE:
      return sizeof(double);
    case eDataType::INT:
      return sizeof(int);
    case eDataType::UNSIGNED_INT:
      return sizeof(unsigned int);
    case eDataType::BOOL:
      return sizeof(bool);
    case eDataType::VECTOR_2_FLOAT:
      return 2 * DataTypeSize(eDataType::FLOAT);
    case eDataType::VECTOR_3_FLOAT:
      return 3 * DataTypeSize(eDataType::FLOAT);
    case eDataType::VECTOR_4_FLOAT:
      return 4 * DataTypeSize(eDataType::FLOAT);
    case eDataType::MATRIX_FLOAT_4X4:
      return 16 * DataTypeSize(eDataType::FLOAT);
  }
}

// ----------------------------------------------------------------------------
