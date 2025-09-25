
set(TARGET_NAME Rendering)

file(GLOB SRC_FILES src/*.cpp)
add_library(${TARGET_NAME} ${SRC_FILES})

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ./api
    PRIVATE
        ./api/Rendering
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC 
        GLEW::GLEW
        glm::glm
)

