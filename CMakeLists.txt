cmake_minimum_required(VERSION 3.21)
project(fem)

set(CMAKE_CXX_STANDARD 17)

include_directories(Module)

set(Eigen3_DIR /home/hansljy/Program/Eigen3/install/share/eigen3/cmake/)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(fem main.cpp)
target_link_libraries (fem Eigen3::Eigen)
