
set(APP_TOOLKIT_LIB Chimia_App)

add_library(${APP_TOOLKIT_LIB}
    src/App.cpp
    src/Application.cpp
    src/GlfwContext.cpp
    src/InputStatus.cpp
    src/OpenGLHelper.cpp
    src/UiContext.cpp
)

target_include_directories(${APP_TOOLKIT_LIB} 
    PRIVATE 
        ./api/App
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

target_compile_features(${APP_TOOLKIT_LIB} PRIVATE cxx_std_17)
