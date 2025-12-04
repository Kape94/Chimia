#include "ShaderCodebase.h"

#include <cassert>
#include <map>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// ShaderCodebaseImpl
// ----------------------------------------------------------------------------

namespace ShaderCodebaseImpl {
std::map<std::string, std::string> codeFragments;

std::string
PreProcess(const std::string& originalCode);

bool
ReplaceInclude(std::string& code);

}

// ----------------------------------------------------------------------------
// ShaderCodebase
// ----------------------------------------------------------------------------

void
ShaderCodebase::RegisterCodes(
  const std::initializer_list<std::pair<std::string, std::string>>&
    shaderCodeList)
{
  for (const auto& tagCodePair : shaderCodeList) {
    const std::string& tag = tagCodePair.first;
    const std::string& code = tagCodePair.second;

    RegisterPieceOfCode(tag, code);
  }
}

// ----------------------------------------------------------------------------

void
ShaderCodebase::RegisterPieceOfCode(const std::string& tag,
                                    const std::string& code)
{
  using namespace ShaderCodebaseImpl;

  const bool codeAlreadyExists = codeFragments.find(tag) != codeFragments.end();
  assert(!codeAlreadyExists &&
         "Registering duplicate tag for shader piece of code");

  codeFragments[tag] = code;
}

// ----------------------------------------------------------------------------

std::string
ShaderCodebase::Code(const std::string& tag)
{
  using namespace ShaderCodebaseImpl;

  auto it = codeFragments.find(tag);
  const bool codeExists = it != codeFragments.end();

  assert(codeExists && "Couldn't find code for tag");

  return PreProcess(it->second);
}

// ----------------------------------------------------------------------------
// ShaderCodebaseImpl
// ----------------------------------------------------------------------------

std::string
ShaderCodebaseImpl::PreProcess(const std::string& originalCode)
{
  std::string expandedCode = originalCode;
  bool handledInclude = ReplaceInclude(expandedCode);
  while (handledInclude) {
    handledInclude = ReplaceInclude(expandedCode);
  }

  return expandedCode;
}

// ----------------------------------------------------------------------------

bool
ShaderCodebaseImpl::ReplaceInclude(std::string& code)
{
  const std::string includeDirective("@include\"");

  const size_t includePos = code.find(includeDirective);
  if (includePos == std::string::npos) {
    return false;
  }

  const size_t includeStart = includePos + includeDirective.length();
  const size_t includeEnd = code.find("\"", includeStart);

  const std::string includeTag = code.substr(includeStart, includeEnd);
  const std::string toBeIncludedCode = ShaderCodebase::Code(includeTag);

  std::string newCode = code.substr(0, includePos);
  newCode += toBeIncludedCode + "\n";
  newCode += code.substr(includeEnd);

  code = newCode;
  return true;
}

// ----------------------------------------------------------------------------