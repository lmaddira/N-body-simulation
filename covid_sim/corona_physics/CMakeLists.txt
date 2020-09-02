add_library(physics_carona carona_physics.h carona_physics.cpp)
find_package(OpenMP)
target_include_directories(physics_carona PUBLIC . )
target_link_libraries(physics_carona PUBLIC OpenMP::OpenMP_CXX carona_body)
