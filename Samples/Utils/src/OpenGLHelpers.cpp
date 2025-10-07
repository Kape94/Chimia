#include "OpenGLHelpers.h"

#include <iostream>

// ----------------------------------------------------------------------------
// Mesh
// ----------------------------------------------------------------------------

Mesh::Mesh()
  : VAO(0)
  , VBO(0)
  , EBO(0)
  , indexCount(0)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

// ====================================================================
// THE REQUIRED FUNCTION: Setup and upload data to the GPU
// ====================================================================
void
Mesh::setup(const std::vector<GLfloat>& vertices,
            const std::vector<GLuint>& indices)
{
  if (vertices.empty() || indices.empty())
    return;

  indexCount = indices.size();

  // 1. Bind the VAO first (it will "record" all subsequent
  // VBO/EBO/AttribPointer calls)
  glBindVertexArray(VAO);

  // 2. Bind and set VBO data (Vertex Positions)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat),
               vertices.data(),
               GL_STATIC_DRAW);

  // 3. Bind and set EBO data (Indices)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(GLuint),
               indices.data(),
               GL_STATIC_DRAW);

  // 4. Configure Vertex Attributes (Tells OpenGL how to interpret the data in
  // the VBO) Attribute location 0: Vertex Position (3 components per vertex)
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  // 5. Unbind the VAO (to prevent accidental modification)
  glBindVertexArray(0);
}

// ====================================================================
// THE DRAWING FUNCTION
// ====================================================================
void
Mesh::draw() const
{
  if (indexCount == 0)
    return;

  // 1. Bind the VAO (This restores all the state from the setup function)
  glBindVertexArray(VAO);

  // 2. Use the indexed drawing call: glDrawElements
  glDrawElements(GL_TRIANGLES,    // Primitive type
                 indexCount,      // Number of indices to draw
                 GL_UNSIGNED_INT, // Type of the indices in the EBO
                 (void*)0 // Offset into the EBO (start from the beginning)
  );

  // 3. Unbind the VAO
  glBindVertexArray(0);
}

// ----------------------------------------------------------------------------
// createShaderProgram
// ----------------------------------------------------------------------------

GLuint
createShaderProgram(const char* vertexShaderSource,
                    const char* fragmentShaderSource)
{
  // --- 1. Vertex Shader Compilation ---
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Check for vertex shader compile errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glDeleteShader(vertexShader); // Clean up the failed shader
    return 0;
  }

  // --- 2. Fragment Shader Compilation ---
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Check for fragment shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); // Clean up both shaders
    return 0;
  }

  // --- 3. Shader Program Linking ---
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    glDeleteProgram(shaderProgram);
    // We still need to detach/delete the shaders even if linking fails
  }

  // --- 4. Cleanup ---
  // The shaders are now linked into the program and are no longer needed.
  // They can be deleted, but the program object keeps the executables.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

// ----------------------------------------------------------------------------