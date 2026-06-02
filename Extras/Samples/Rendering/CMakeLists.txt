
include(../../Utils/CMakeUtils.cmake)

add_library(Samples_Rendering_DefaultDeps INTERFACE)

target_link_libraries(Samples_Rendering_DefaultDeps
    INTERFACE
        Extras_Utils
        Chimia_Rendering  
)

target_compile_features(Samples_Rendering_DefaultDeps INTERFACE cxx_std_17)

# 1) Minimal

add_executable(Samples_Rendering_ex1 ex1-minimal.cpp)
target_link_libraries(Samples_Rendering_ex1 PRIVATE Samples_Rendering_DefaultDeps)

# 2) Texture

add_executable(Samples_Rendering_ex2 ex2-texture.cpp)
target_link_libraries(Samples_Rendering_ex2 PRIVATE Samples_Rendering_DefaultDeps)
copy_assets_dir(Samples_Rendering_ex2)

# 3) Framebuffer

add_executable(Samples_Rendering_ex3 ex3-framebuffer.cpp)
target_link_libraries(Samples_Rendering_ex3 PRIVATE Samples_Rendering_DefaultDeps)
copy_assets_dir(Samples_Rendering_ex3)

# 4) Shader uniforms 

add_executable(Samples_Rendering_ex4 ex4-shaderUniforms.cpp)
target_link_libraries(Samples_Rendering_ex4 PRIVATE Samples_Rendering_DefaultDeps)

# 5) Instancing

add_executable(Samples_Rendering_ex5 ex5-instancing.cpp)
target_link_libraries(Samples_Rendering_ex5 PRIVATE Samples_Rendering_DefaultDeps)

# 6) Regular buffer

add_executable(Samples_Rendering_ex6 ex6-regularBuffer.cpp)
target_link_libraries(Samples_Rendering_ex6 PRIVATE Samples_Rendering_DefaultDeps)

# 7) Regular buffer with instancing

add_executable(Samples_Rendering_ex7 ex7-regularBufferInstanced.cpp)
target_link_libraries(Samples_Rendering_ex7 PRIVATE Samples_Rendering_DefaultDeps)

# 8) Regular buffer with instancing + instanced subdata

add_executable(Samples_Rendering_ex8 ex8-instancedBufferSubData.cpp)
target_link_libraries(Samples_Rendering_ex8 PRIVATE Samples_Rendering_DefaultDeps)

# 9) Regular buffer with instancing using transformation matrices

add_executable(Samples_Rendering_ex9 ex9-instancedBufferWithTransform.cpp)
target_link_libraries(Samples_Rendering_ex9 PRIVATE Samples_Rendering_DefaultDeps)

# 10) Regular buffer sub data with varying vertex size

add_executable(Samples_Rendering_ex10 ex10-regularBufferSubData.cpp)
target_link_libraries(Samples_Rendering_ex10 PRIVATE Samples_Rendering_DefaultDeps)

# 11) Indexed buffer sub data 

add_executable(Samples_Rendering_ex11 ex11-indexedBufferSubData.cpp)
target_link_libraries(Samples_Rendering_ex11 PRIVATE Samples_Rendering_DefaultDeps)

# 12) Reusable vertex buffer simple usage

add_executable(Samples_Rendering_ex12 ex12-reusableVertexBufferSimple.cpp)
target_link_libraries(Samples_Rendering_ex12 PRIVATE Samples_Rendering_DefaultDeps)

# 13) Reusable vertex buffer with instancing

add_executable(Samples_Rendering_ex13 ex13-reusableVertexBufferWithInstancing.cpp)
target_link_libraries(Samples_Rendering_ex13 PRIVATE Samples_Rendering_DefaultDeps)

# 14) Color blending

add_executable(Samples_Rendering_ex14 ex14-colorBlending.cpp)
target_link_libraries(Samples_Rendering_ex14 PRIVATE Samples_Rendering_DefaultDeps)

