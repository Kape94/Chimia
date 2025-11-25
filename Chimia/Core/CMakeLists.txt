
set(TARGET_NAME Chimia_Core)

add_library(${TARGET_NAME}
    src/Diagnostics.cpp
    src/Types.cpp
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ./api/Core/
    PUBLIC
        ./api/
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)