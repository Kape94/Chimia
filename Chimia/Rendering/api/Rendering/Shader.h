#pragma once

//---------------------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Rendering/DataLayout.h"
#include "RenderingNamespaceDefs.h"
#include "Texture2D.h"
#include "TextureUnit.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <map>
#include <memory>
#include <string>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Shader
{
public:
  NON_COPYABLE(Shader)

  Shader(Shader&& other);
  Shader& operator=(Shader&& other);

  ~Shader();

  static std::shared_ptr<Shader> Create(const std::string& vertexShaderCode,
                                        const std::string& fragmentShaderCode,
                                        const DataLayout& dataLayout);

  void SetUniform(const std::string& name, const int value);
  void SetUniform(const std::string& name, const float value);
  void SetTexture(const std::string& name,
                  const Texture2DInstance& texture,
                  const TextureUnit& unit);
  void SetUniform(const std::string& name, const glm::mat4x4& matrix);
  void SetUniform(const std::string& name, const glm::vec3& vector);

private:
  Shader() = default;

  friend class BufferPrivate;
  void Use() const;
  const DataLayout& GetDataLayout() const;
  unsigned GetLocationOfAttribute(const std::string& attributeName) const;

  void Clear();
  int GetUniformLocation(const std::string& name) const;

  unsigned CreateVertexShader(const char* vertexShaderCode);

  unsigned CreateFragmentShader(const char* fragmentShaderCode);

  unsigned CreateShader(const unsigned shaderType, const char* shaderCode);

  void CheckShaderCompilationStatus(const int shaderID);

  void CheckProgramLinkStatus(const int programID);

  void LinkProgram(const unsigned vShaderID, const unsigned fShaderID);

  void PopulateAttributeLocations(const DataLayout& dataLayout);

  using AttributeLocationTable = std::vector<std::pair<std::string, int>>;
  AttributeLocationTable m_attributeLocationTable;

  using TextureEntry = struct
  {
    Texture2DInstance texture;
    TextureUnit unit;
  };
  std::map<std::string, TextureEntry> m_inUseTextures;

  DataLayout m_dataLayout;
  unsigned m_programId = 0;
};

using ShaderInstance = std::shared_ptr<Shader>;

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
