
set(TARGET_NAME Extras_Utils)
add_library(${TARGET_NAME}
    src/ImageUtils.cpp
    src/OpenGLHelpers.cpp
    src/SamplesUtils.cpp
    src/Window.cpp
)

# TODO: This link shouldn't be public at all
target_link_libraries(${TARGET_NAME} 
    PUBLIC
        Chimia_Media
    PRIVATE
        glfw
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/api
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Utils
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)