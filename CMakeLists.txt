cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O3")

include_directories(./Module)
file(GLOB_RECURSE src ./Module/*.cc)
#
#set (ADDITIONAL_CMAKE_PATH
#    "${PROJECT_SOURCE_DIR}/cmake/"
#)
#
#list(PREPEND CMAKE_MODULE_PATH ${ADDITIONAL_CMAKE_PATH})
#list(PREPEND CMAKE_PREFIX_PATH ${ADDITIONAL_CMAKE_PATH})

set(SuiteSparse_DIR /home/hansljy/Program/SuiteSparse/build)
set(Eigen3_DIR /home/hansljy/Program/Eigen3/install/share/eigen3/cmake/)
set(OsqpEigen_DIR /home/hansljy/Program/osqp-eigen-build/lib/cmake/OsqpEigen/)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package (spdlog REQUIRED)
find_package (OsqpEigen REQUIRED)
find_package (SuiteSparse 5.10 NO_MODULE)

add_executable(fem main.cpp ${src})
target_link_libraries(fem Eigen3::Eigen)
target_link_libraries(fem cppunit)
target_link_libraries(fem spdlog::spdlog)
target_link_libraries(fem OsqpEigen::OsqpEigen)
target_link_libraries(fem SuiteSparse::CHOLMOD)

message("Success!")