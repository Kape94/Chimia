
SET(TARGET_NAME Chimia_Draw3D)

add_library(${TARGET_NAME}
    src/Camera.cpp
    src/Draw3D.cpp
    src/Draw3DPrivate.cpp
    src/Illumination.cpp
    src/LitTriangle.cpp
    src/Model.cpp
    src/ModelRenderingComponent.cpp
    src/Renderers.cpp
    src/Resources.cpp
    src/ResourcesManager.cpp
    src/Shaders.cpp
    src/Triangle.cpp
    src/TriangleMeshComponent.cpp
    src/VertexColoredRendererImpl.cpp

    src/batch/BatchUtils.cpp
    src/batch/ModelBatch.cpp
    src/batch/StaticModel.cpp
    src/batch/StaticTriangles.cpp
    src/batch/TriangleBatch.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        glm::glm
        Chimia_Core
    PRIVATE
        Chimia_Bits
        Chimia_Rendering
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ./api/Draw3D/
        ./src/
        ./src/batch/
    PUBLIC
        ./api/
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)