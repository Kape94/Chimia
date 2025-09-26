
SET(TARGET_NAME Chimia_Draw3D)

add_library(${TARGET_NAME}
    src/Draw3D.cpp
    src/Triangle.cpp
    src/TriangleDetail.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        glm::glm
    PRIVATE
        Bits
        Rendering
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ./api/Draw3D/
    PUBLIC
        ./api/
)