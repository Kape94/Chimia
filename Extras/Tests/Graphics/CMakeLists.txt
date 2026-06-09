
include(../../Utils/CMakeUtils.cmake)
include(../CMakeTestsUtils.cmake)

# -----------------------------------------------------------------------------

add_library(GraphicsTestsDeps INTERFACE)

target_link_libraries(GraphicsTestsDeps 
    INTERFACE
        Chimia_Draw3D
        TestsUtils
)

target_compile_features(GraphicsTestsDeps INTERFACE cxx_std_17)

# -----------------------------------------------------------------------------

# 1) Basic triangle

add_executable(Test_Graphics_1_basic test1_basic.cpp)
target_link_libraries(Test_Graphics_1_basic PRIVATE GraphicsTestsDeps)
copy_golden_artifacts_dir(Test_Graphics_1_basic)
