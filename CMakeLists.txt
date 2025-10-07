cmake_minimum_required(VERSION 3.30)
project(ChimiaLib)

find_package(assimp CONFIG REQUIRED)  
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_subdirectory(Chimia)

option(BUILD_SAMPLES OFF)
if (BUILD_SAMPLES)
    add_subdirectory(Samples)
endif()