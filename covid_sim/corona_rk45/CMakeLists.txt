set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)
add_library(carona_rk45 carona_rk45.h carona_rk45.cpp)
target_link_libraries(carona_rk45 RK4 carona_body)
target_include_directories(carona_rk45 PUBLIC . OpenMP::OpenMP_CXX)
