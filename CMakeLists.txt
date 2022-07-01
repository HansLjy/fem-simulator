cmake_minimum_required(VERSION 3.21)

project(fem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")

include_directories(./Module)
include_directories(./Frontend)
file(GLOB_RECURSE src ./Module/*.cc ./Frontend/*.cc)

set(SuiteSparse_DIR /home/hansljy/Program/SuiteSparse/build)
set(Eigen3_DIR /home/hansljy/Program/Eigen3/install/share/eigen3/cmake/)
set(OsqpEigen_DIR /home/hansljy/Program/osqp-eigen-build/lib/cmake/OsqpEigen/)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(spdlog REQUIRED)
find_package(OsqpEigen REQUIRED)
find_package(SuiteSparse 5.10 NO_MODULE)
find_package(OpenGL REQUIRED)

#include(cmake/get_cpm.cmake)
#
#option(LIBIGL_GLFW "Build target igl::glfw" ON)
#option(LIBIGL_IMGUI "Build target igl::imgui" ON)
#option(LIBIGL_OPENGL "Build target igl::opengl" ON)
#
#CPMAddPackage(
#        NAME igl
#        GIT_REPOSITORY http://git.what996.com/libigl/libigl.git
#        GIT_TAG 66b3ef2253e765d0ce0db74cec91bd706e5ba176
#)
#
#include_directories(${igl_SOURCE_DIR}/include)

add_executable(fem main.cpp ${src})
target_link_libraries(fem Eigen3::Eigen)
target_link_libraries(fem cppunit)
target_link_libraries(fem spdlog::spdlog)
target_link_libraries(fem OsqpEigen::OsqpEigen)
target_link_libraries(fem SuiteSparse::CHOLMOD)
#target_link_libraries(fem igl::core)
#target_link_libraries(fem igl::imgui)
#target_link_libraries(fem igl::glfw)
#target_link_libraries(fem glad::glad)

#message("Success!")