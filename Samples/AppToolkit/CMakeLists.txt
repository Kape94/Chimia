
add_executable(ex1-minimal ex1-minimal.cpp)
target_link_libraries(ex1-minimal PRIVATE AppToolkit)

add_executable(ex2-simpleUI ex2-simpleUI.cpp)
target_link_libraries(ex2-simpleUI PRIVATE AppToolkit)

add_executable(ex3-drawing ex3-drawing.cpp)
target_link_libraries(ex3-drawing PRIVATE AppToolkit)

add_executable(ex4-drawingUIAndInput ex4-drawingUIAndInput.cpp)
target_link_libraries(ex4-drawingUIAndInput PRIVATE AppToolkit)
