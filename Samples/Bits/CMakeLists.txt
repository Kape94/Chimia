add_executable(Samples_Bits_ex1 ex1-buffer.cpp)
target_link_libraries(Samples_Bits_ex1 Bits)

add_executable(Samples_Bits_ex2 ex2-memoryPool.cpp)
target_link_libraries(Samples_Bits_ex2 Bits)

add_executable(Samples_Bits_ex3 ex3-memoryPoolLoadTest.cpp)
target_link_libraries(Samples_Bits_ex3 Bits)

add_executable(Samples_Bits_ex4 ex4-threadPool.cpp)
target_link_libraries(Samples_Bits_ex4 Bits)

add_executable(Samples_Bits_ex5 ex5-pipeUsage.cpp)
target_link_libraries(Samples_Bits_ex5 Bits)

add_executable(Samples_Bits_ex6 ex6-processUsage.cpp)
target_link_libraries(Samples_Bits_ex6 Bits)

add_executable(Samples_Bits_ex7 ex7-processInfoUsage.cpp)
target_link_libraries(Samples_Bits_ex7 Bits)
