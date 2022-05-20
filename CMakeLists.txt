cmake_minimum_required(VERSION 3.21)
project(fem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
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

include(FetchContent)
#FetchContent_Declare(
#        ccd_wrapper
#        GIT_REPOSITORY https://github.com/Continuous-Collision-Detection/CCD-Wrapper
#        GIT_TAG 23907dadf3e1eef606e38450ada4aa4f96fd9f71
#        GIT_SHALLOW FALSE
#
#)
#FetchContent_MakeAvailable(ccd_wrapper)

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
#target_link_libraries(fem ccd_wrapper::ccd_wrapper)

#message("Success!")