
set(TARGET_NAME RenderLib)

file(GLOB SRC_FILES src/*.cpp)
add_library(${TARGET_NAME} ${SRC_FILES})

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ./api
    PRIVATE
        ./api/RenderLib
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC 
        GLEW::GLEW
        glm::glm
)

option(BUILD_SAMPLES OFF)
if (BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
