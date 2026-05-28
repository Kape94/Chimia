
#include "Utils/OpenGLHelpers.h"
#include "Utils/SamplesUtils.h"
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
  Window w(1280, 960, "Misc ex2");

  glewExperimental = true;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glDeleteShader(vertexShader);
    return 0;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    glDeleteProgram(shaderProgram);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat),
               vertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(GLuint),
               indices.data(),
               GL_STATIC_DRAW);

  indexCount = indices.size();

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  while (!w.ShouldClose()) {
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);

    w.Swap();
    w.PollEvents();
  }

  SamplesUtils::SaveScreenshot(w, "newTest.bmp");

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  return 0;
}