
set(TARGET_NAME Chimia_Core)

add_library(${TARGET_NAME}
    src/DataBuffer.cpp
    src/Diagnostics.cpp
    src/Mesh.cpp
    src/SchemaDataBuffer.cpp
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ./api/Core/
    PUBLIC
        ./api/
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        glm::glm
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)