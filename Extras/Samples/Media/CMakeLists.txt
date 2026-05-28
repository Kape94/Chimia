
add_library(Samples_Media_DefaultDeps INTERFACE)

target_link_libraries(Samples_Media_DefaultDeps
  INTERFACE
    Chimia_Media
    Samples_Utils 
    GLEW::GLEW 
    glm::glm
)

target_compile_features(Samples_Media_DefaultDeps INTERFACE cxx_std_17)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.mtl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.mtl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

# 1) Simple example

add_executable(Samples_Media_ex1 ex1-basic.cpp)
target_link_libraries(Samples_Media_ex1 PRIVATE Samples_Media_DefaultDeps)

add_custom_command(
    TARGET Samples_Media_ex1
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.obj"
    "$<TARGET_FILE_DIR:Samples_Media_ex1>/assets/Untitled.obj"
)

add_custom_command(
    TARGET Samples_Media_ex1
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/Untitled.mtl"
    "$<TARGET_FILE_DIR:Samples_Media_ex1>/assets/Untitled.mtl"
)

# 2) Two meshes with colored vertices

add_executable(Samples_Media_ex2 ex2-colored.cpp)
target_link_libraries(Samples_Media_ex2 PRIVATE Samples_Media_DefaultDeps)
target_compile_features(Samples_Media_ex2 PRIVATE cxx_std_20)

add_custom_command(
    TARGET Samples_Media_ex2
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.obj"
    "$<TARGET_FILE_DIR:Samples_Media_ex2>/assets/ObjectPair.obj"
)

add_custom_command(
    TARGET Samples_Media_ex2
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/ObjectPair.mtl"
    "$<TARGET_FILE_DIR:Samples_Media_ex2>/assets/ObjectPair.mtl"
)