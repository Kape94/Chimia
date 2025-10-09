
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

# 1) Simple example

add_executable(Samples_Importer_ex1 ex1-basic.cpp)
target_link_libraries(Samples_Importer_ex1 PRIVATE Samples_Importer_DefaultDeps)

add_custom_command(
    TARGET Samples_Importer_ex1
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.obj"
    "$<TARGET_FILE_DIR:Samples_Importer_ex1>/assets/Untitled.obj"
)

add_custom_command(
    TARGET Samples_Importer_ex1
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.mtl"
    "$<TARGET_FILE_DIR:Samples_Importer_ex1>/assets/Untitled.mtl"
)

# 2) Two meshes with colored vertices

add_executable(Samples_Importer_ex2 ex2-colored.cpp)
target_link_libraries(Samples_Importer_ex2 PRIVATE Samples_Importer_DefaultDeps)
target_compile_features(Samples_Importer_ex2 PRIVATE cxx_std_20)

add_custom_command(
    TARGET Samples_Importer_ex2
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.obj"
    "$<TARGET_FILE_DIR:Samples_Importer_ex2>/assets/ObjectPair.obj"
)

add_custom_command(
    TARGET Samples_Importer_ex2
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.mtl"
    "$<TARGET_FILE_DIR:Samples_Importer_ex2>/assets/ObjectPair.mtl"
)