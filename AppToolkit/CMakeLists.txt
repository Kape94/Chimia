
set(APP_TOOLKIT_LIB AppToolkit)

add_library(${APP_TOOLKIT_LIB}
    src/Application.cpp
    src/AppToolkit.cpp
    src/GlfwContext.cpp
    src/InputStatus.cpp
    src/OpenGLHelper.cpp
    src/UiContext.cpp
)

target_include_directories(${APP_TOOLKIT_LIB} 
    PRIVATE 
        ./api/AppToolkit
    PUBLIC
        ./api/ 
)

target_link_libraries(${APP_TOOLKIT_LIB} 
    PUBLIC 
        GLEW::GLEW
        glfw
        glm::glm
        imgui::imgui
)

option(BUILD_SAMPLES OFF)
if (${BUILD_SAMPLES})
  add_subdirectory(samples)
endif()
