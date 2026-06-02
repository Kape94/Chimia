
include(../../CMakeUtils.cmake)

add_library(Samples_Media_DefaultDeps INTERFACE)

target_link_libraries(Samples_Media_DefaultDeps
  INTERFACE
    Chimia_Media
    Extras_Utils 
    GLEW::GLEW 
    glm::glm
)

target_compile_features(Samples_Media_DefaultDeps INTERFACE cxx_std_17)

# 1) Simple example

add_executable(Samples_Media_ex1 ex1-basic.cpp)
target_link_libraries(Samples_Media_ex1 PRIVATE Samples_Media_DefaultDeps)
copy_assets_dir(Samples_Media_ex1)

# 2) Two meshes with colored vertices

add_executable(Samples_Media_ex2 ex2-colored.cpp)
target_link_libraries(Samples_Media_ex2 PRIVATE Samples_Media_DefaultDeps)
target_compile_features(Samples_Media_ex2 PRIVATE cxx_std_20)
copy_assets_dir(Samples_Media_ex2)
