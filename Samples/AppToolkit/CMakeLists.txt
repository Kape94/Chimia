
add_executable(Samples_App_ex1 ex1-minimal.cpp)
target_link_libraries(Samples_App_ex1 AppToolkit)

add_executable(Samples_App_ex2 ex2-simpleUI.cpp)
target_link_libraries(Samples_App_ex2 AppToolkit)

add_executable(Samples_App_ex3 ex3-drawing.cpp)
target_link_libraries(Samples_App_ex3 AppToolkit)

add_executable(Samples_App_ex4 ex4-drawingUIAndInput.cpp)
target_link_libraries(Samples_App_ex4 AppToolkit)
