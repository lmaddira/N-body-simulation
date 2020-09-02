add_library(function function.h function.cpp)
find_package(OpenMP)
target_include_directories(function PUBLIC . )
target_link_libraries(function PUBLIC OpenMP::OpenMP_CXX body)
