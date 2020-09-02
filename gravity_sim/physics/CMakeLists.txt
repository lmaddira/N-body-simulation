add_library(physics physics.h physics.cpp)
find_package(OpenMP)
target_include_directories(physics PUBLIC . )
target_link_libraries(physics PUBLIC OpenMP::OpenMP_CXX body)