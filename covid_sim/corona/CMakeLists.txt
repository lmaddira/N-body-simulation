cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(corona)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(corona MACOSX_BUNDLE main.cpp)
target_link_libraries(corona carona_rk45 physics_carona fslazywindow ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

