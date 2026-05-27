#include "Media/Model3DImport.h"

#include "Utils/OpenGLHelpers.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include "GL/glew.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

// ----------------------------------------------------------------------------

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 transform;

    out vec3 vertexColor;

    void main()
    {
        gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
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
  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";

  const std::string modelPath = assetsDir + "Untitled.obj";

  std::vector<float> vertices;
  std::vector<unsigned> indices;

  std::vector<Chimia::Media::Mesh> meshes =
    Chimia::Media::ImportMeshes(modelPath);

  for (const auto& v : meshes.front().vertices) {
    vertices.insert(vertices.end(), { v.x, v.y, v.z });
  }
  for (const auto& i : meshes.front().indices) {
    indices.push_back(i);
  }

  Window w(1280, 960, "Importer ex1");

  glewExperimental = true;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  const int shaderID =
    createShaderProgram(vertexShaderSource, fragmentShaderSource);

  Mesh m;
  m.setup(vertices, indices);

  glm::vec3 cameraPos{ 0.0f, 1.0f, 5.0f };
  glm::mat4x4 view =
    glm::lookAt(cameraPos, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
  glm::mat4x4 proj = glm::perspective(45.0f, 1.0f, 0.01f, 100.0f);

  glm::mat4x4 transform = proj * view;

  const int transformLoc = glGetUniformLocation(shaderID, "transform");
  while (!w.ShouldClose()) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderID);
    glProgramUniformMatrix4fv(shaderID,
                              transformLoc,
                              1,
                              GL_FALSE /*transpose*/,
                              glm::value_ptr(transform));
    m.draw();

    w.Swap();

    cameraPos =
      glm::rotate(glm::identity<glm::mat4x4>(), 0.005f, { 0.0f, 1.0f, 0.0f }) *
      glm::vec4(cameraPos, 1.0f);
    view = glm::lookAt(cameraPos, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
    transform = proj * view;

    w.PollEvents();
  }

  return 0;
}