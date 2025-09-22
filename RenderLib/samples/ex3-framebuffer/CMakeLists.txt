set(TARGET_NAME RenderLibExamples_ex3)
add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} 
  PRIVATE
    RenderLibExamples_Utils
    RenderLib  
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/solar-flare.jpg
                   ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Copying resources")