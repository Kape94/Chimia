#include "Media/MediaTypes.h"
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
#include <vector>

// ----------------------------------------------------------------------------

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aCol;

    uniform mat4 transform;

    out vec3 vertexColor;

    void main()
    {
        gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        vertexColor = aCol;
    }
    )";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 vertexColor;
    void main()
    {
        FragColor = vec4(vertexColor, 1.0f);
    }
    )";

int
main(int argc, char** argv)
{
  Window w(1280, 960, "Importer ex2");

  glewExperimental = true;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  const int shaderID =
    createShaderProgram(vertexShaderSource, fragmentShaderSource);

  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";

  const std::string modelPath = assetsDir + "ObjectPair.obj";

  std::vector<Chimia::Media::Mesh> meshes =
    Chimia::Media::ImportMeshes(modelPath);
  auto bufferDatas =
    Chimia::Media::PackBufferDataFromMeshes(meshes,
                                            { .includeColors = true,
                                              .includeNormals = false,
                                              .includeTexCoords = false });

  auto adjustShaderAttribs = []() {
    glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(GLfloat),
                          (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  };

  std::vector<Mesh> renderMeshes;
  for (const auto& data : bufferDatas) {

    Mesh& createdMesh = renderMeshes.emplace_back();
    createdMesh.setup(data.vertexData, data.indices);
    createdMesh.adjust(adjustShaderAttribs);
  }

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
    glProgramUniformMatrix4fv(
      shaderID, transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    for (const auto& m : renderMeshes) {
      m.draw();
    }

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