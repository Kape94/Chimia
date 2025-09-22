set(TARGET_NAME RenderLibExamples_ex2)
add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} 
  PRIVATE
    RenderLibExamples_Utils
    RenderLib  
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/blue-light-style.jpg
                   ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Copying resources")

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/solar-flare.jpg
                   ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Copying resources")
