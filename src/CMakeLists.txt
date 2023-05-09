project(Switch LANGUAGES CXX)

cmake_minimum_required(VERSION 3.22)

include_directories(/usr/include/python3.10/ inc)



SET(GCC_COVERAGE_COMPILE_FLAGS "-DWITHOUT_NUMPY")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(switch switch.cpp)

target_link_libraries(switch python3.10)
