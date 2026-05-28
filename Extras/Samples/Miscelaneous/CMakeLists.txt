
add_library(Samples_Misc_DefaultDeps INTERFACE)

target_link_libraries(Samples_Misc_DefaultDeps
  INTERFACE
    Samples_Utils
    GLEW::GLEW 
    glm::glm
)

target_compile_features(Samples_Misc_DefaultDeps INTERFACE cxx_std_17)

add_executable(Samples_Misc_ex1 ex1-openGLSimple.cpp)
target_link_libraries(Samples_Misc_ex1 PRIVATE Samples_Misc_DefaultDeps)

add_executable(Samples_Misc_ex2 ex2-glScreenshotPOC.cpp)
target_link_libraries(Samples_Misc_ex2 PRIVATE Samples_Misc_DefaultDeps)