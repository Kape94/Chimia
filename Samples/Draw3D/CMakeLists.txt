add_library(Samples_Draw3D_Dependencies INTERFACE)
target_link_libraries(Samples_Draw3D_Dependencies INTERFACE Chimia_Draw3D Samples_Utils)
target_compile_features(Samples_Draw3D_Dependencies INTERFACE cxx_std_17)

add_executable(Samples_Draw3D_ex1 ex1-triangles.cpp)
target_link_libraries(Samples_Draw3D_ex1 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex2 ex2-vertexColoredTriangles.cpp)
target_link_libraries(Samples_Draw3D_ex2 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex3 ex3-camera.cpp)
target_link_libraries(Samples_Draw3D_ex3 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex4 ex4-basicIllumination.cpp)
target_link_libraries(Samples_Draw3D_ex4 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex5 ex5-vertexColoredRenderer.cpp)
target_link_libraries(Samples_Draw3D_ex5 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex6 ex6-vertexColoredRendererModel.cpp)
target_link_libraries(Samples_Draw3D_ex6 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex7 ex7-vertexColoredRendererIndexedTriangles.cpp)
target_link_libraries(Samples_Draw3D_ex7 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex8 ex8-vertexColoredRendererStaticTriangles.cpp)
target_link_libraries(Samples_Draw3D_ex8 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex9 ex9-vertexColoredRendererStaticModels.cpp)
target_link_libraries(Samples_Draw3D_ex9 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex10 ex10-basicIlluminationLitRenderer.cpp)
target_link_libraries(Samples_Draw3D_ex10 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex11 ex11-litRendererWithModel.cpp)
target_link_libraries(Samples_Draw3D_ex11 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex12 ex12-basicIlluminationPhongLitRenderer.cpp)
target_link_libraries(Samples_Draw3D_ex12 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex13 ex13-illuminationVertexColored.cpp)
target_link_libraries(Samples_Draw3D_ex13 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex14 ex14-litVertexColoredRendererWithModel.cpp)
target_link_libraries(Samples_Draw3D_ex14 PRIVATE Samples_Draw3D_Dependencies)

add_executable(Samples_Draw3D_ex15 ex15-texturedTriangle.cpp)
target_link_libraries(Samples_Draw3D_ex15 PRIVATE Samples_Draw3D_Dependencies)

add_custom_command(
    TARGET Samples_Draw3D_ex15
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/blue-light-style.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex15>/assets/blue-light-style.jpg")

add_custom_command(
    TARGET Samples_Draw3D_ex15
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/solar-flare.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex15>/assets/solar-flare.jpg"
)

add_executable(Samples_Draw3D_ex16 ex16-manyTexturedTriangles.cpp)
target_link_libraries(Samples_Draw3D_ex16 PRIVATE Samples_Draw3D_Dependencies)

add_custom_command(
    TARGET Samples_Draw3D_ex16
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/blue-light-style.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex16>/assets/blue-light-style.jpg")

add_custom_command(
    TARGET Samples_Draw3D_ex16
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/solar-flare.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex16>/assets/solar-flare.jpg"
)

add_executable(Samples_Draw3D_ex17 ex17-texturedTrianglesMisc.cpp)
target_link_libraries(Samples_Draw3D_ex17 PRIVATE Samples_Draw3D_Dependencies)

add_custom_command(
    TARGET Samples_Draw3D_ex17
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/blue-light-style.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex17>/assets/blue-light-style.jpg")

add_custom_command(
    TARGET Samples_Draw3D_ex17
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/solar-flare.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex17>/assets/solar-flare.jpg"
)

add_executable(Samples_Draw3D_ex18 ex18-texturedLit.cpp)
target_link_libraries(Samples_Draw3D_ex18 PRIVATE Samples_Draw3D_Dependencies)

add_custom_command(
    TARGET Samples_Draw3D_ex18
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/box.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex18>/assets/box.jpg")

add_executable(Samples_Draw3D_ex19 ex19-texturedLitMisc.cpp)
target_link_libraries(Samples_Draw3D_ex19 PRIVATE Samples_Draw3D_Dependencies)

add_custom_command(
    TARGET Samples_Draw3D_ex19
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/box.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex19>/assets/box.jpg")

add_custom_command(
    TARGET Samples_Draw3D_ex19
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/solar-flare.jpg"
    "$<TARGET_FILE_DIR:Samples_Draw3D_ex19>/assets/solar-flare.jpg"
)