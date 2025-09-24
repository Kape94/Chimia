
set(TARGET_NAME Samples_Utils)
add_library(${TARGET_NAME}
    src/SamplesUtils.cpp
    src/Window.cpp
)

target_link_libraries(${TARGET_NAME} 
    PRIVATE
        glfw
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/api
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Utils
)
