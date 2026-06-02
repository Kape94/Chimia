
include(../../Utils/CMakeUtils.cmake)

function(copy_golden_artifacts_dir TARGET_NAME)
    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/goldenArtifacts"
        "$<TARGET_FILE_DIR:${TARGET_NAME}>/goldenArtifacts")
endfunction()

# -----------------------------------------------------------------------------

add_library(RenderingTestsDeps INTERFACE)

target_link_libraries(RenderingTestsDeps 
    INTERFACE
        Chimia_Rendering
        TestsUtils
)

target_compile_features(RenderingTestsDeps INTERFACE cxx_std_17)

# -----------------------------------------------------------------------------

# 1) Basic triangle

add_executable(Test_Rendering_1_basic test1_basic.cpp)
target_link_libraries(Test_Rendering_1_basic PRIVATE RenderingTestsDeps)
copy_golden_artifacts_dir(Test_Rendering_1_basic)

# 2) Textured triangle

add_executable(Test_Rendering_2_texture test2_texture.cpp)
target_link_libraries(Test_Rendering_2_texture PRIVATE RenderingTestsDeps)
copy_assets_dir(Test_Rendering_2_texture)
copy_golden_artifacts_dir(Test_Rendering_2_texture)