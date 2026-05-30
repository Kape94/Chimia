
add_library(RenderingTestsDeps INTERFACE)

target_link_libraries(RenderingTestsDeps 
    INTERFACE
        Chimia_Rendering
        Extras_Utils
        Tester
)

target_compile_features(RenderingTestsDeps INTERFACE cxx_std_17)

# -----------------------------------------------------------------------------

add_executable(Test_Rendering_1_basic test1_basic.cpp)
target_link_libraries(Test_Rendering_1_basic PRIVATE RenderingTestsDeps)

add_custom_command(
    TARGET Test_Rendering_1_basic
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/goldenArtifacts/test1_basic.bmp"
    "$<TARGET_FILE_DIR:Test_Rendering_1_basic>/goldenArtifacts/test1_basic.bmp"
)
