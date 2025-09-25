
add_library(Samples_Rendering_DefaultDeps INTERFACE)

target_link_libraries(Samples_Rendering_DefaultDeps
    INTERFACE
        Samples_Utils
        Rendering  
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/blue-light-style.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/solar-flare.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

# 1) Minimal

add_executable(Samples_Rendering_ex1 ex1-minimal.cpp)
target_link_libraries(Samples_Rendering_ex1 PRIVATE Samples_Rendering_DefaultDeps)

# 2) Texture

add_executable(Samples_Rendering_ex2 ex2-texture.cpp)
target_link_libraries(Samples_Rendering_ex2 PRIVATE Samples_Rendering_DefaultDeps)

# 3) Framebuffer

add_executable(Samples_Rendering_ex3 ex3-framebuffer.cpp)
target_link_libraries(Samples_Rendering_ex3 PRIVATE Samples_Rendering_DefaultDeps)

# 4) Shader uniforms 

add_executable(Samples_Rendering_ex4 ex4-shaderUniforms.cpp)
target_link_libraries(Samples_Rendering_ex4 PRIVATE Samples_Rendering_DefaultDeps)

# 5) Instancing

add_executable(Samples_Rendering_ex5 ex5-instancing.cpp)
target_link_libraries(Samples_Rendering_ex5 PRIVATE Samples_Rendering_DefaultDeps)

# 6) Regular buffer

add_executable(Samples_Rendering_ex6 ex6-regularBuffer.cpp)
target_link_libraries(Samples_Rendering_ex6 PRIVATE Samples_Rendering_DefaultDeps)
