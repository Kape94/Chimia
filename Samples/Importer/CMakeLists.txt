
add_library(Samples_Importer_DefaultDeps INTERFACE)

target_link_libraries(Samples_Importer_DefaultDeps
  INTERFACE
    Chimia_Importer
    Samples_Utils 
    GLEW::GLEW 
    glm::glm
)

target_compile_features(Samples_Importer_DefaultDeps INTERFACE cxx_std_17)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.mtl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.mtl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

add_executable(Samples_Importer_ex1 ex1-basic.cpp)
target_link_libraries(Samples_Importer_ex1 PRIVATE Samples_Importer_DefaultDeps)

add_executable(Samples_Importer_ex2 ex2-colored.cpp)
target_link_libraries(Samples_Importer_ex2 PRIVATE Samples_Importer_DefaultDeps)
