#include "ShaderCodebase.h"

#include "Core/Diagnostics.h"

#include <cassert>
#include <map>
#include <set>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// ShaderCodebaseImpl
// ----------------------------------------------------------------------------

namespace ShaderCodebaseImpl {
std::map<std::string, std::string> codeFragments;
const std::string includeDirective("@include \"");

std::string
OriginalCode(const std::string& tag);

std::string
PreProcess(const std::string& originalCode);

bool
ReplaceNextInclude(std::string& code,
                   std::set<std::string>& alreadyIncludedCodes);

using CodeParts = struct
{
  std::string includeTag;
  std::string codeBeforeInclude;
  std::string codeAfterInclude;
};
CodeParts
SplitCodeParts(const std::string& code, const size_t includePos);

std::string
FetchCodeToInclude(const std::string& includeTag,
                   std::set<std::string>& alreadyIncludedCodes);

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

  return PreProcess(OriginalCode(tag));
}

// ----------------------------------------------------------------------------
// ShaderCodebaseImpl
// ----------------------------------------------------------------------------

std::string
ShaderCodebaseImpl::OriginalCode(const std::string& tag)
{
  auto it = codeFragments.find(tag);
  const bool codeExists = it != codeFragments.end();

  if (!codeExists) {
    std::string msg = "Couldn't find code for tag ";
    msg += tag;
    Chimia::Diagnostics::Error(1, msg);
  }

  return it->second;
}

// ----------------------------------------------------------------------------

std::string
ShaderCodebaseImpl::PreProcess(const std::string& originalCode)
{
  std::set<std::string> alreadyIncludedCodes;

  std::string expandedCode = originalCode;
  bool handledInclude = ReplaceNextInclude(expandedCode, alreadyIncludedCodes);
  while (handledInclude) {
    handledInclude = ReplaceNextInclude(expandedCode, alreadyIncludedCodes);
  }

  return expandedCode;
}

// ----------------------------------------------------------------------------

bool
ShaderCodebaseImpl::ReplaceNextInclude(
  std::string& code,
  std::set<std::string>& alreadyIncludedCodes)
{
  const size_t includePos = code.find(includeDirective);
  if (includePos == std::string::npos) {
    return false;
  }

  const CodeParts parts = SplitCodeParts(code, includePos);
  const std::string& includeTag = parts.includeTag;

  const std::string codeToBeIncluded =
    FetchCodeToInclude(includeTag, alreadyIncludedCodes);

  std::string newCode = parts.codeBeforeInclude;
  newCode += codeToBeIncluded + "\n";
  newCode += parts.codeAfterInclude;

  code = newCode;
  return true;
}

// ----------------------------------------------------------------------------

ShaderCodebaseImpl::CodeParts
ShaderCodebaseImpl::SplitCodeParts(const std::string& code,
                                   const size_t includePos)
{
  const size_t includeStart = includePos + includeDirective.length();
  const size_t includeEnd = code.find('\"', includeStart);
  const size_t includeTagLenght = includeEnd - includeStart;

  CodeParts parts;
  parts.includeTag = code.substr(includeStart, includeTagLenght);
  parts.codeBeforeInclude = code.substr(0, includePos);
  parts.codeAfterInclude = code.substr(includeEnd + 1);

  return parts;
}

// ----------------------------------------------------------------------------

std::string
ShaderCodebaseImpl::FetchCodeToInclude(
  const std::string& includeTag,
  std::set<std::string>& alreadyIncludedCodes)
{
  const bool codeWasAlreadyIncluded =
    alreadyIncludedCodes.count(includeTag) > 0;

  if (!codeWasAlreadyIncluded) {
    alreadyIncludedCodes.insert(includeTag);
  }

  const std::string skipInclude = "// Skipping include of " + includeTag +
                                  " as it was already included previously\n";
  const std::string codeToBeIncluded =
    codeWasAlreadyIncluded ? skipInclude : OriginalCode(includeTag);

  return codeToBeIncluded;
}

// ----------------------------------------------------------------------------
