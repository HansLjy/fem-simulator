cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0")

include_directories(../Module)
file(GLOB_RECURSE src ../Module/*.cc)
file(GLOB_RECURSE testsrc src/*.cc)

set(Eigen3_DIR /home/hansljy/Program/Eigen3/install/share/eigen3/cmake/)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package (spdlog REQUIRED)
find_package(OsqpEigen REQUIRED)

add_executable(Test Test.cc ${src} ${testsrc})
target_link_libraries(Test Eigen3::Eigen)
target_link_libraries(Test cppunit)
target_link_libraries(Test spdlog::spdlog)
target_link_libraries(Test OsqpEigen::OsqpEigen)