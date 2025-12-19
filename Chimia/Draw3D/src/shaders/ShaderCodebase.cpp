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

struct Directive
{
  std::string start = "";
  std::string end = "";
};

const Directive INCLUDE_DIRECTIVE{ "@include \"", "\"" };
const Directive EMBED_DIRECTIVE{ "@embed(", ")" };

std::string
OriginalCode(const std::string& tag);

std::string
PreProcess(const std::string& originalCode,
           const std::map<std::string, std::string>& embeddedCodes);

std::string
ApplyIncludes(const std::string& originalCode);

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
SplitCodeParts(const std::string& code,
               const size_t directivePos,
               const Directive& directive);

std::string
FetchCodeToInclude(const std::string& includeTag,
                   std::set<std::string>& alreadyIncludedCodes);

std::string
ApplyEmbeddedCode(const std::string originalCode,
                  const std::map<std::string, std::string>& embeddedCodes);

bool
ReplaceNextEmbedding(std::string& code,
                     const std::map<std::string, std::string>& embeddedCodes);

}

// ----------------------------------------------------------------------------
// ShaderCodebase
// ----------------------------------------------------------------------------

void
ShaderCodebase::AddCodes(
  const std::initializer_list<std::pair<std::string, std::string>>&
    shaderCodeList)
{
  for (const auto& tagCodePair : shaderCodeList) {
    const std::string& tag = tagCodePair.first;
    const std::string& code = tagCodePair.second;

    AddToCodebase(tag, code);
  }
}

// ----------------------------------------------------------------------------

void
ShaderCodebase::AddToCodebase(const std::string& tag, const std::string& code)
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
  return Code(tag, {});
}

// ----------------------------------------------------------------------------

std::string
ShaderCodebase::Code(const std::string& tag,
                     const std::map<std::string, std::string>& embeddedCodes)
{
  using namespace ShaderCodebaseImpl;

  return PreProcess(OriginalCode(tag), embeddedCodes);
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
ShaderCodebaseImpl::PreProcess(
  const std::string& originalCode,
  const std::map<std::string, std::string>& embeddedCodes)
{
  const std::string codeWithExpandedIncludes = ApplyIncludes(originalCode);
  return ApplyEmbeddedCode(codeWithExpandedIncludes, embeddedCodes);
}

// ----------------------------------------------------------------------------

std::string
ShaderCodebaseImpl::ApplyIncludes(const std::string& originalCode)
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
  const size_t includePos = code.find(INCLUDE_DIRECTIVE.start);
  if (includePos == std::string::npos) {
    return false;
  }

  const CodeParts parts = SplitCodeParts(code, includePos, INCLUDE_DIRECTIVE);
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
                                   const size_t directivePos,
                                   const Directive& directive)
{
  const size_t includeStart = directivePos + directive.start.length();
  const size_t includeEnd = code.find(directive.end, includeStart);
  const size_t includeTagLenght = includeEnd - includeStart;

  CodeParts parts;
  parts.includeTag = code.substr(includeStart, includeTagLenght);
  parts.codeBeforeInclude = code.substr(0, directivePos);
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

std::string
ShaderCodebaseImpl::ApplyEmbeddedCode(
  const std::string originalCode,
  const std::map<std::string, std::string>& embeddedCodes)
{
  std::string expandedCode = originalCode;
  bool replacedEmbbeding = ReplaceNextEmbedding(expandedCode, embeddedCodes);
  while (replacedEmbbeding) {
    replacedEmbbeding = ReplaceNextEmbedding(expandedCode, embeddedCodes);
  }

  return expandedCode;
}

// ----------------------------------------------------------------------------

bool
ShaderCodebaseImpl::ReplaceNextEmbedding(
  std::string& code,
  const std::map<std::string, std::string>& embeddedCodes)
{
  const size_t embedPos = code.find(EMBED_DIRECTIVE.start);
  if (embedPos == std::string::npos) {
    return false;
  }

  const CodeParts parts = SplitCodeParts(code, embedPos, EMBED_DIRECTIVE);
  const std::string& embedTag = parts.includeTag;

  auto it = embeddedCodes.find(embedTag);
  const std::string codeToEmbbed =
    it != embeddedCodes.end() ? it->second : "\n//";

  std::string newCode = parts.codeBeforeInclude;
  newCode += codeToEmbbed + "\n";
  newCode += parts.codeAfterInclude;

  code = newCode;
  return true;
}

// ----------------------------------------------------------------------------
