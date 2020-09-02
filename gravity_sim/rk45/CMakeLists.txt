set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)
add_library(rk45 rk45.h rk45.cpp)
target_link_libraries(rk45 function RK4)
target_include_directories(rk45 PUBLIC . OpenMP::OpenMP_CXX)
add_subdirectory(RK_methods)
