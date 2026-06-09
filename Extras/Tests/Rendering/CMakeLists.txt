
include(../../Utils/CMakeUtils.cmake)
include(../CMakeTestsUtils.cmake)

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

# 3) Framebuffer

add_executable(Test_Rendering_3_framebuffer test3_framebuffer.cpp)
target_link_libraries(Test_Rendering_3_framebuffer PRIVATE RenderingTestsDeps)
copy_assets_dir(Test_Rendering_3_framebuffer)
copy_golden_artifacts_dir(Test_Rendering_3_framebuffer)

# 4) Shader uniform

add_executable(Test_Rendering_4_ShaderUniform test4_shaderUniform.cpp)
target_link_libraries(Test_Rendering_4_ShaderUniform PRIVATE RenderingTestsDeps)
copy_golden_artifacts_dir(Test_Rendering_4_ShaderUniform)

# 5) Instanced buffer

add_executable(Test_Rendering_5_InstancedBuffer test5_instancedBuffer.cpp)
target_link_libraries(Test_Rendering_5_InstancedBuffer PRIVATE RenderingTestsDeps)
copy_golden_artifacts_dir(Test_Rendering_5_InstancedBuffer)

# 6) Buffer

add_executable(Test_Rendering_6_buffer test6_buffer.cpp)
target_link_libraries(Test_Rendering_6_buffer PRIVATE RenderingTestsDeps)
copy_golden_artifacts_dir(Test_Rendering_6_buffer)

# 7) ReusableBuffer

add_executable(Test_Rendering_7_reusableBuffer test7_reusableBuffer.cpp)
target_link_libraries(Test_Rendering_7_reusableBuffer PRIVATE RenderingTestsDeps)
copy_golden_artifacts_dir(Test_Rendering_7_reusableBuffer)

# 8) Color blending

add_executable(Test_Rendering_8_colorBlending test8_colorBlending.cpp)
target_link_libraries(Test_Rendering_8_colorBlending PRIVATE RenderingTestsDeps)
copy_golden_artifacts_dir(Test_Rendering_8_colorBlending)