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

  Shader(const char* vertexShaderCode, const char* fragmentShaderCode);

  ~Shader();

  void Create(const char* vertexShaderCode, const char* fragmentShaderCode);

  void Use();

  void Clear();

  void SetUniform(const std::string& name, const TextureUnit& unit);

  void SetUniform(const std::string& name, const glm::mat4x4& matrix);

  void SetUniform(const std::string& name, const glm::vec3& vector);

private:
  int GetUniformLocation(const std::string& name) const;

  unsigned id = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
