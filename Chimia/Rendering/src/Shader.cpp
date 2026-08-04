#include "Shader.h"

#include "BufferPrivate.h"
#include "DataLayout.h"
#include "OpenGLDefs.h"

#include "Core/Diagnostics.h"

#include <cassert>
#include <glm/gtc/type_ptr.hpp>

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

Shader::Shader(Shader&& other)
  : m_programId(other.m_programId)
  , m_dataLayout(std::move(other.m_dataLayout))
{
  other.m_programId = 0;
}

//---------------------------------------------------------------------------------------

Shader&
Shader::operator=(Shader&& other)
{
  m_programId = other.m_programId;

  other.m_programId = 0;

  return *this;
}

//---------------------------------------------------------------------------------------

Shader::~Shader()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
Shader::Create(const std::string& vertexShaderCode,
               const std::string& fragmentShaderCode,
               const DataLayout& dataLayout)
{
  Clear();

  const unsigned vShaderID = CreateVertexShader(vertexShaderCode.c_str());
  const unsigned fShaderID = CreateFragmentShader(fragmentShaderCode.c_str());

  LinkProgram(vShaderID, fShaderID);

  glDeleteShader(vShaderID);
  glDeleteShader(fShaderID);

  m_dataLayout = dataLayout;
  PopulateAttributeLocations(dataLayout);
}

//---------------------------------------------------------------------------------------

unsigned
Shader::CreateVertexShader(const char* vertexShaderCode)
{
  return CreateShader(GL_VERTEX_SHADER, vertexShaderCode);
}

//---------------------------------------------------------------------------------------

unsigned
Shader::CreateFragmentShader(const char* fragmentShaderCode)
{
  return CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
}

//---------------------------------------------------------------------------------------

unsigned
Shader::CreateShader(const unsigned shaderType, const char* shaderCode)
{
  const unsigned shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, &shaderCode, nullptr);
  glCompileShader(shaderID);
  CheckShaderCompilationStatus(shaderID);

  return shaderID;
}

//---------------------------------------------------------------------------------------

void
Shader::CheckShaderCompilationStatus(const int shaderID)
{
  int success;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    constexpr unsigned INFO_LOG_SIZE = 512;
    char infoLog[INFO_LOG_SIZE];
    glGetShaderInfoLog(shaderID, INFO_LOG_SIZE, NULL, infoLog);

    std::string errorMsg = "Shader compilation failed\n";
    errorMsg += infoLog;
    Diagnostics::Error(1, errorMsg);
  }
}

//---------------------------------------------------------------------------------------

void
Shader::LinkProgram(const unsigned vShaderID, const unsigned fShaderID)
{
  m_programId = glCreateProgram();
  glAttachShader(m_programId, vShaderID);
  glAttachShader(m_programId, fShaderID);
  glLinkProgram(m_programId);

  CheckProgramLinkStatus(m_programId);
}

//---------------------------------------------------------------------------------------

void
Shader::CheckProgramLinkStatus(const int programID)
{
  int success;
  glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
  if (!success) {
    constexpr unsigned INFO_LOG_SIZE = 512;
    char infoLog[INFO_LOG_SIZE];
    glGetProgramInfoLog(m_programId, INFO_LOG_SIZE, NULL, infoLog);

    std::string errorMsg = "Shader program linking failed\n";
    errorMsg += infoLog;
    Diagnostics::Error(1, errorMsg);
  }
}

//---------------------------------------------------------------------------------------

void
Shader::PopulateAttributeLocations(const DataLayout& dataLayout)
{
  dataLayout.ForEachSpec([&](const auto& spec) {
    const std::string name = spec.name;

    const int posLocation = glGetAttribLocation(m_programId, name.c_str());
    if (posLocation == -1) {
      assert(false && "Couldn't find location for shader attribute specified");
    }

    m_attributeLocationTable.push_back({ name, posLocation });
  });
}

//---------------------------------------------------------------------------------------

void
Shader::Use()
{
  glUseProgram(m_programId);
}

//---------------------------------------------------------------------------------------

void
Shader::SetUniform(const std::string& name, const int value)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    glProgramUniform1i(m_programId, location, value);
  }
}

//---------------------------------------------------------------------------------------

void
Shader::SetUniform(const std::string& name, const float value)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    glProgramUniform1f(m_programId, location, value);
  }
}

//---------------------------------------------------------------------------------------

void
Shader::SetTexture(const std::string& name,
                   const Texture2D& texture,
                   const TextureUnit& unit)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    BufferPrivate::UseTexture(texture, unit);
    glProgramUniform1i(m_programId, location, static_cast<unsigned>(unit));
  }
}

//---------------------------------------------------------------------------------------

void
Shader::SetUniform(const std::string& name, const glm::mat4x4& matrix)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    glProgramUniformMatrix4fv(
      m_programId, location, 1, GL_FALSE /*transpose*/, glm::value_ptr(matrix));
  }
}

//---------------------------------------------------------------------------------------

void
Shader::SetUniform(const std::string& name, const glm::vec3& vector)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    glProgramUniform3f(m_programId, location, vector.x, vector.y, vector.z);
  }
}

//---------------------------------------------------------------------------------------

int
Shader::GetUniformLocation(const std::string& name) const
{
  return glGetUniformLocation(m_programId, name.c_str());
}

//---------------------------------------------------------------------------------------

void
Shader::Clear()
{
  if (m_programId != 0) {
    glDeleteProgram(m_programId);
    m_programId = 0;
  }
}

// ----------------------------------------------------------------------------

const DataLayout&
Shader::GetDataLayout() const
{
  return m_dataLayout;
}

//---------------------------------------------------------------------------------------

unsigned
Shader::GetLocationOfAttribute(const std::string& attributeName) const
{
  for (const auto& dataToLocation : m_attributeLocationTable) {
    const std::string& dataName = dataToLocation.first;
    const unsigned dataLocation = static_cast<unsigned>(dataToLocation.second);
    if (dataName == attributeName) {
      return dataLocation;
    }
  }

  assert(false && "Didn't find location for searched attribute");
  return 0;
}

//---------------------------------------------------------------------------------------