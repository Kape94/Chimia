
set(TARGET_NAME RenderLibExamples_ex5)
add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} 
  PRIVATE
    RenderLibExamples_Utils
    RenderLib  
)