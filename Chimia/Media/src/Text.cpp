#include "Text.h"

#include <fstream>
#include <string>

// ----------------------------------------------------------------------------

USING_CHIMIA_MEDIA_NAMESPACE

// ----------------------------------------------------------------------------

Text::Text(Text&& other)
  : m_lines(std::move(other.m_lines))
{
}

// ----------------------------------------------------------------------------

Text&
Text::operator=(Text&& other)
{
  m_lines = std::move(other.m_lines);

  return *this;
}

// ----------------------------------------------------------------------------

Text
Text::FromFile(const std::string& filePath)
{
  std::ifstream inFile(filePath);
  if (!inFile.good()) {
    return Text();
  }

  Text text;

  std::string buffer;
  while (std::getline(inFile, buffer)) {
    text.AddLine(buffer);
  }

  return text;
}

// ----------------------------------------------------------------------------

void
Text::AddLine(const std::string& textLine)
{
  m_lines.push_back(textLine);
}

// ----------------------------------------------------------------------------

bool
Text::IsEqual(const Text& other) const
{
  const std::vector<std::string>& otherText = other.m_lines;

  if (m_lines.size() != otherText.size()) {
    return false;
  }

  for (size_t i = 0; i < m_lines.size(); ++i) {
    const std::string& thisLine = m_lines[i];
    const std::string& otherLine = otherText[i];
    if (thisLine != otherLine) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------

void
Text::Save(const std::string& filePath) const
{
  std::ofstream file(filePath);
  if (!file.good()) {
    return;
  }

  for (const std::string& line : m_lines) {
    file << line;
  }
}

// ----------------------------------------------------------------------------