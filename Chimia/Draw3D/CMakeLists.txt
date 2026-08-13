
SET(TARGET_NAME Chimia_Draw3D)

add_library(${TARGET_NAME}
    src/Camera.cpp
    src/Config.cpp
    src/Draw3D.cpp
    src/Draw3DPrivate.cpp
    src/Illumination.cpp
    src/Model.cpp
    src/ModelRendering.cpp
    src/Pipeline.cpp
    src/Pipelines.cpp
    src/ResourceGroupHelper.cpp
    src/Transition.cpp
    src/TransitionRendering.cpp
    src/Triangle.cpp
    
    src/batch/BatchUtils.cpp
    src/batch/ImmediateModelInstancesBatch.cpp
    src/batch/ImmediateTransitionsBatch.cpp
    src/batch/ImmediateTrianglesBatch.cpp
    src/batch/RetainedModelInstancesBatch.cpp
    src/batch/RetainedTrianglesBatch.cpp
    src/batch/ShaderBindingsTemplate.cpp

    src/renderers/DataBindingProvider.cpp
    src/renderers/GenericRenderer.cpp
    src/renderers/ModelRenderingComponent.cpp
    src/renderers/Renderers.cpp
    src/renderers/TransitionRenderingComponent.cpp
    src/renderers/TriangleMeshComponent.cpp
    src/renderers/RenderersUtils.cpp

    src/renderers/defaultRenderers/Color4.cpp
    src/renderers/defaultRenderers/Normal3.cpp
    src/renderers/defaultRenderers/TexCoord2.cpp
    src/renderers/defaultRenderers/Color4Normal3.cpp
    src/renderers/defaultRenderers/Color4TexCoord2.cpp
    src/renderers/defaultRenderers/Normal3TexCoord2.cpp
    src/renderers/defaultRenderers/Color4Normal3TexCoord2.cpp

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
        ./src/resources/
        ./src/shaders/
    PUBLIC
        ./api/
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)