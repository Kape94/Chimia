
include(../../Utils/CMakeUtils.cmake)
include(../CMakeTestsUtils.cmake)

# -----------------------------------------------------------------------------

add_library(GraphicsTestsDeps
    Common/QuadsDrawingFixture.cpp
    Common/TestTrianglesDrawing.cpp
)

target_include_directories(
    GraphicsTestsDeps
        PUBLIC
            ./Common
)

target_link_libraries(GraphicsTestsDeps 
    PUBLIC
        Chimia_Draw3D
        TestsUtils
)

target_compile_features(GraphicsTestsDeps PUBLIC cxx_std_17)

# -----------------------------------------------------------------------------

# 1) Basic triangle

add_executable(Test_Graphics_1_basic test1_basic.cpp)
target_link_libraries(Test_Graphics_1_basic PRIVATE GraphicsTestsDeps)
copy_golden_artifacts_dir(Test_Graphics_1_basic)

# 2) Triangles in immediate mode

add_executable(Test_Graphics_2_trianglesImmediate test2_trianglesImmediate.cpp)
target_link_libraries(Test_Graphics_2_trianglesImmediate PRIVATE GraphicsTestsDeps)
copy_golden_artifacts_dir(Test_Graphics_2_trianglesImmediate)
copy_assets_dir(Test_Graphics_2_trianglesImmediate)

# 3) Batching

add_executable(Test_Graphics_3_batching test3_batching.cpp)
target_link_libraries(Test_Graphics_3_batching PRIVATE GraphicsTestsDeps)
copy_golden_artifacts_dir(Test_Graphics_3_batching)
copy_assets_dir(Test_Graphics_3_batching)

# 4) Triangles in retained mode

add_executable(Test_Graphics_4_trianglesRetained test4_trianglesRetained.cpp)
target_link_libraries(Test_Graphics_4_trianglesRetained PRIVATE GraphicsTestsDeps)
copy_golden_artifacts_dir(Test_Graphics_4_trianglesRetained)
copy_assets_dir(Test_Graphics_4_trianglesRetained)
