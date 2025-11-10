#pragma once

//---------------------------------------------------------------------------------------

#include <initializer_list>
#include <vector>

#include "RenderingNamespaceDefs.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class ShaderAttribute
{
public:
  ShaderAttribute(const ShaderAttribute& other);
  ShaderAttribute& operator=(const ShaderAttribute& other);

  static ShaderAttribute Float(const unsigned location,
                               const unsigned nEntries);

  static ShaderAttribute Double(const unsigned location,
                                const unsigned nEntries);

  static ShaderAttribute Int(const unsigned location, const unsigned nEntries);

  static ShaderAttribute UnsignedInt(const unsigned location,
                                     const unsigned nEntries);

  static ShaderAttribute Short(const unsigned location,
                               const unsigned nEntries);

  static ShaderAttribute UnsignedShort(const unsigned location,
                                       const unsigned nEntries);

  static ShaderAttribute Byte(const unsigned location, const unsigned nEntries);

  static ShaderAttribute UnsignedByte(const unsigned location,
                                      const unsigned nEntries);

  unsigned Location() const;
  unsigned NEntries() const;
  unsigned DataType() const;
  unsigned DataSizeInBytes() const;

private:
  ShaderAttribute(const unsigned _location,
                  const unsigned _nEntries,
                  const unsigned _dataType);

  unsigned location = 0;
  unsigned nEntries = 0;
  unsigned dataType = 0;
  unsigned dataSizeInBytes = 0;
};

//---------------------------------------------------------------------------------------

class ShaderAttributes : public std::vector<ShaderAttribute>
{
public:
  ShaderAttributes() = default;

  ShaderAttributes(const std::initializer_list<ShaderAttribute>& attributes)
    : std::vector<ShaderAttribute>(attributes)
  {
  }

  unsigned ComputeTotalSizeOfAttributes() const
  {
    unsigned totalSize = 0;
    for (const ShaderAttribute& attr : *this) {
      totalSize += attr.DataSizeInBytes();
    }
    return totalSize;
  }

  unsigned ComputeTotalEntriesOfAttributes() const
  {
    unsigned totalEntries = 0;
    for (const ShaderAttribute& attr : *this) {
      totalEntries += attr.NEntries();
    }
    return totalEntries;
  }
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------