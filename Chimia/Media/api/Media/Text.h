#pragma once

#include "MediaNamespaceDefs.h"

#include "Core/ClassDefs.h"

#include <string>
#include <vector>

BEGIN_CHIMIA_MEDIA_NAMESPACE

class Text
{
public:
  NON_COPYABLE(Text)

  Text() = default;
  ~Text() = default;

  Text(Text&& other);
  Text& operator=(Text&& other);

  static Text FromFile(const std::string& filePath);

  void AddLine(const std::string& textLine);

  bool IsEqual(const Text& other) const;
  void Save(const std::string& filePath) const;

private:
  std::vector<std::string> m_lines;
};

END_CHIMIA_MEDIA_NAMESPACE