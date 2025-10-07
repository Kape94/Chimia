
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.mtl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

add_executable(Samples_Importer_ex1 ex1-basic.cpp)
target_link_libraries(Samples_Importer_ex1 Chimia_Importer Samples_Utils GLEW::GLEW glm::glm)
