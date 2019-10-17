cmake_minimum_required(VERSION 3.0)
project(Mathc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -march=native)  # march=native for best performance
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function")

add_executable(vecmath examples/vecmath_example.c)
target_include_directories(vecmath PUBLIC include)
target_link_libraries(vecmath m)

add_executable(matmath examples/matmath_example.c)
target_include_directories(matmath PUBLIC include)
target_link_libraries(matmath m)