# -----------------------------------------------------------------------------
# Testing helper
# -----------------------------------------------------------------------------

add_library(TestsUtils
    Tester.cpp
    TestsUtils.cpp
)

target_include_directories(TestsUtils
    PUBLIC
        ./
)

target_link_libraries(TestsUtils
    PUBLIC
        Extras_Utils
)

# -----------------------------------------------------------------------------
# Test cases
# -----------------------------------------------------------------------------

add_subdirectory(Rendering)
add_subdirectory(Graphics)

# -----------------------------------------------------------------------------
# Main test runner
# -----------------------------------------------------------------------------

set(TESTS RunTests)

add_executable(${TESTS} 
    RunTests.cpp
)

target_link_libraries(${TESTS}
    PRIVATE
        Chimia_Rendering
        Extras_Utils
)

target_compile_features(${TESTS} PRIVATE cxx_std_17)

# -----------------------------------------------------------------------------