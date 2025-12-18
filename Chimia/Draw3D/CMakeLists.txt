
SET(TARGET_NAME Chimia_Draw3D)

add_library(${TARGET_NAME}
    src/Camera.cpp
    src/Config.cpp
    src/Draw3D.cpp
    src/Draw3DPrivate.cpp
    src/Illumination.cpp
    src/Model.cpp
    src/ModelRendering.cpp
    src/ResourceGroupHelper.cpp
    src/Triangle.cpp
    
    src/batch/BatchUtils.cpp
    src/batch/ModelBatch.cpp
    src/batch/StaticModel.cpp
    src/batch/StaticTriangles.cpp
    src/batch/TriangleBatch.cpp

    src/renderers/GenericRenderer.cpp
    src/renderers/ModelRenderingComponent.cpp
    src/renderers/Renderers.cpp
    src/renderers/TriangleMeshComponent.cpp
    src/renderers/VertexLayoutAttributes.cpp

    src/renderers/default/ColoredTexturedLitRendererImpl.cpp
    src/renderers/default/ColoredTexturedRendererImpl.cpp
    src/renderers/default/LitRendererImpl.cpp
    src/renderers/default/LitWithVertexColorRendererImpl.cpp
    src/renderers/default/TexturedLitRendererImpl.cpp
    src/renderers/default/TexturedRendererImpl.cpp
    src/renderers/default/VertexColoredRendererImpl.cpp

    src/resources/Resources.cpp
    src/resources/ResourcesGroup.cpp 
    src/resources/ResourcesManager.cpp

    src/shaders/Shaders.cpp
    src/shaders/ShaderCodebase.cpp
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
        ./src/renderers/
        ./src/renderers/default/
        ./src/resources/
        ./src/shaders/
    PUBLIC
        ./api/
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)