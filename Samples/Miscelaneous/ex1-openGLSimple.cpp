
#include "Utils/OpenGLHelpers.h"
#include "Utils/Window.h"

#include "GL/glew.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

// ----------------------------------------------------------------------------

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec3 vertexColor;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        vertexColor = aPos;
    }
    )";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 vertexColor;
    void main()
    {
        FragColor = vec4(vertexColor, 1.0f); // A magenta color
    }
    )";

int
main(int argc, char** argv)
{
  Window w(1280, 960, "Misc ex1");

  glewExperimental = true;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  // ==========================================================================
  // SHADER CREATION
  // ==========================================================================
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

  // ==========================================================================
  // BUFFER CREATION
  // ==========================================================================
  std::vector<float> vertices{
    0.5f,  0.5f,  0.0f, // top right
    0.5f,  -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f  // top left
  };

  std::vector<unsigned> indices{
    // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  GLsizei indexCount;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

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

  // ==========================================================================
  // RENDERING
  // ==========================================================================

  while (!w.ShouldClose()) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

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

    w.Swap();
    w.PollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  return 0;
}