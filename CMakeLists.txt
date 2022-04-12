cmake_minimum_required(VERSION 3.21)
project(fem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -fopenmp")

include_directories(Module)
file(GLOB_RECURSE src Module/*.cc)

set(Eigen3_DIR /home/hansljy/Program/Eigen3/install/share/eigen3/cmake/)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package (spdlog REQUIRED)

add_executable(fem main.cpp ${src})
target_link_libraries(fem Eigen3::Eigen)
target_link_libraries(fem spdlog::spdlog)

message("Fuck")
