#pragma once

//---------------------------------------------------------------------------------------

#include "RenderingNamespaceDefs.h"
#include "TextureUnit.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <string>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Shader
{
public:
  Shader() = default;

  Shader(const std::string& vertexShaderCode,
         const std::string& fragmentShaderCode);
  Shader(const char* vertexShaderCode, const char* fragmentShaderCode);

  Shader(const Shader& other) = delete;
  Shader& operator=(const Shader& other) = delete;

  Shader(Shader&& other);
  Shader& operator=(Shader&& other);

  ~Shader();

  void Create(const std::string& vertexShaderCode,
              const std::string& fragmentShaderCode);
  void Create(const char* vertexShaderCode, const char* fragmentShaderCode);

  void Use();

  void Clear();

  void SetUniform(const std::string& name, const int value);

  void SetUniform(const std::string& name, const float value);

  void SetUniform(const std::string& name, const TextureUnit& unit);

  void SetUniform(const std::string& name, const glm::mat4x4& matrix);

  void SetUniform(const std::string& name, const glm::vec3& vector);

private:
  int GetUniformLocation(const std::string& name) const;

  unsigned CreateVertexShader(const char* vertexShaderCode);

  unsigned CreateFragmentShader(const char* fragmentShaderCode);

  unsigned CreateShader(const unsigned shaderType, const char* shaderCode);

  void CheckShaderCompilationStatus(const int shaderID);

  void CheckProgramLinkStatus(const int programID);

  void LinkProgram(const unsigned vShaderID, const unsigned fShaderID);

  unsigned m_programId = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
