cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(test_blue_screens)
add_executable(test_blue_screens MACOSX_BUNDLE main.cpp)
target_link_libraries(test_blue_screens function fslazywindow)
include(CTest)
add_test(NAME alfa_version_test COMMAND test_blue_screens)

