
set(TARGET_NAME Chimia_Importer)

add_library(${TARGET_NAME}
    src/Model3DImport.cpp
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        ./api/
    PRIVATE
        ./api/Importer/
)

target_link_libraries(${TARGET_NAME} 
    PRIVATE 
        assimp::assimp
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)