
set(TARGET_NAME Chimia_Media)

add_library(${TARGET_NAME}
    src/Image.cpp
    src/ImageFormat.cpp
    src/Model3DImport.cpp
    src/Text.cpp
)

target_include_directories(${TARGET_NAME}
    SYSTEM PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        ./api/
    PRIVATE
        ./api/Media/
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC
        Chimia_Core
    PRIVATE 
        assimp::assimp
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)