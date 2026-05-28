
set(TESTS RunTests)

add_executable(${TESTS} 
    DummyTests.cpp
    RunTests.cpp
)

target_link_libraries(${TESTS}
    PRIVATE
        Chimia_Rendering
        Extras_Utils
)

target_compile_features(${TESTS} PRIVATE cxx_std_17)