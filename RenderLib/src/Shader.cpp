#include "Shader.h"

#include "OpenGLDefs.h"

#include <glm/gtc/type_ptr.hpp>

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

Shader::Shader(const char* vertexShaderCode, const char* fragmentShaderCode)
{
  Create(vertexShaderCode, fragmentShaderCode);
}

//---------------------------------------------------------------------------------------

Shader::~Shader()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
Shader::Create(const char* vertexShaderCode, const char* fragmentShaderCode)
{
  Clear();

  const unsigned vShaderID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShaderID, 1, &vertexShaderCode, nullptr);
  glCompileShader(vShaderID);

  const unsigned fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShaderID, 1, &fragmentShaderCode, nullptr);
  glCompileShader(fShaderID);

  id = glCreateProgram();
  glAttachShader(id, vShaderID);
  glAttachShader(id, fShaderID);
  glLinkProgram(id);

  glDeleteShader(vShaderID);
  glDeleteShader(fShaderID);
}

//---------------------------------------------------------------------------------------

void
Shader::Use()
{
  glUseProgram(id);
}

//---------------------------------------------------------------------------------------

void
Shader::SetUniform(const std::string& name, const TextureUnit& unit)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    glProgramUniform1i(id, location, static_cast<unsigned>(unit));
  }
}

//---------------------------------------------------------------------------------------

void
Shader::SetUniform(const std::string& name, const glm::mat4x4& matrix)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    glUniformMatrix4fv(
      location, 1, GL_FALSE /*transpose*/, glm::value_ptr(matrix));
  }
}

//---------------------------------------------------------------------------------------

void
Shader::SetUniform(const std::string& name, const glm::vec3& vector)
{
  const int location = GetUniformLocation(name);
  if (location != -1) {
    glUniform3f(location, vector.x, vector.y, vector.z);
  }
}

//---------------------------------------------------------------------------------------

int
Shader::GetUniformLocation(const std::string& name) const
{
  return glGetUniformLocation(id, name.c_str());
}

//---------------------------------------------------------------------------------------

void
Shader::Clear()
{
  if (id != 0) {
    glDeleteProgram(id);
    id = 0;
  }
}

//---------------------------------------------------------------------------------------
