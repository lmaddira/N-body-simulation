cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(gravity_sim)

add_executable(gravity_sim MACOSX_BUNDLE main.cpp)
target_link_libraries(gravity_sim function rk45 physics fslazywindow)

