add_executable(ex1-buffer ex1-buffer.cpp)
target_link_libraries(ex1-buffer Misc)

add_executable(ex2-memoryPool ex2-memoryPool.cpp)
target_link_libraries(ex2-memoryPool Misc)

add_executable(ex3-memoryPoolLoadTest ex3-memoryPoolLoadTest.cpp)
target_link_libraries(ex3-memoryPoolLoadTest Misc)

add_executable(ex4-threadPool ex4-threadPool.cpp)
target_link_libraries(ex4-threadPool Misc)

add_executable(ex5-pipeUsage ex5-pipeUsage.cpp)
target_link_libraries(ex5-pipeUsage Misc)

add_executable(ex6-processUsage ex6-processUsage.cpp)
target_link_libraries(ex6-processUsage Misc)

add_executable(ex7-processInfoUsage ex7-processInfoUsage.cpp)
target_link_libraries(ex7-processInfoUsage Misc)
