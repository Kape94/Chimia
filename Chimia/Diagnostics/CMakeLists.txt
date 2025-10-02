
set(TARGET_NAME Chimia_Diagnostics)

add_library(${TARGET_NAME}
    src/Diagnostics.cpp
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ./api/Diagnostics/
    PUBLIC
        ./api/
)
