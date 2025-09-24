add_executable(Samples_Misc_ex1 ex1-buffer.cpp)
target_link_libraries(Samples_Misc_ex1 Misc)

add_executable(Samples_Misc_ex2 ex2-memoryPool.cpp)
target_link_libraries(Samples_Misc_ex2 Misc)

add_executable(Samples_Misc_ex3 ex3-memoryPoolLoadTest.cpp)
target_link_libraries(Samples_Misc_ex3 Misc)

add_executable(Samples_Misc_ex4 ex4-threadPool.cpp)
target_link_libraries(Samples_Misc_ex4 Misc)

add_executable(Samples_Misc_ex5 ex5-pipeUsage.cpp)
target_link_libraries(Samples_Misc_ex5 Misc)

add_executable(Samples_Misc_ex6 ex6-processUsage.cpp)
target_link_libraries(Samples_Misc_ex6 Misc)

add_executable(Samples_Misc_ex7 ex7-processInfoUsage.cpp)
target_link_libraries(Samples_Misc_ex7 Misc)
