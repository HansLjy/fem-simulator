cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")

include_directories(../Module)
file(GLOB_RECURSE src ../Module/*.cc)

set(Eigen3_DIR /home/hansljy/Program/Eigen3/install/share/eigen3/cmake/)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package (spdlog REQUIRED)

add_executable(OptimizerTest OptimizerTest.cc ${src})
target_link_libraries(OptimizerTest Eigen3::Eigen)
target_link_libraries(OptimizerTest cppunit)
target_link_libraries(OptimizerTest spdlog::spdlog)