cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(fem)

set(CMAKE_CXX_STANDARD 17)
#if (DEBUG)
#    set(CMAKE_BUILD_TYPE "Debug")
#    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
#else()
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#endif()

include_directories(./Module)
include_directories(./Frontend)
file(GLOB_RECURSE src ./Module/*.cc ./Frontend/*.cc)

find_package(SuiteSparse 5.10 NO_MODULE)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG 76fb40d95455f249bd70824ecfcae7a8f0930fa3
)
include_directories(${spdlog_SOURCE_DIR}/include)

CPMAddPackage(
        NAME eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 06a458a13d7cbea54a2cbf6a491f6bbf1f64c8ce
        PATCH_COMMAND git am ${CMAKE_SOURCE_DIR}/patch/eigen.patch
)
include_directories(${eigen_SOURCE_DIR}/include)

CPMAddPackage(
        NAME OsqpEigen
        GIT_REPOSITORY https://github.com/robotology/osqp-eigen.git
        GIT_TAG 83812bd0a56bbb656cac7016b307845e4a0ed11e
        PATCH_COMMAND git am ${CMAKE_SOURCE_DIR}/patch/osqp-eigen.patch
)
include_directories(${osqpeigen_SOURCE_DIR}/include)

CPMAddPackage(
        NAME SuiteSparse
        GIT_REPOSITORY https://github.com/sergiud/SuiteSparse.git
        GIT_TAG 2357545619575293f466d6a7d545834d05495392
)

add_subdirectory(Renderer)
include_directories(Renderer/src)

add_executable(fem main.cpp App.cc ${src})
target_link_libraries(fem Eigen3::Eigen)
target_link_libraries(fem cppunit)
target_link_libraries(fem spdlog::spdlog)
target_link_libraries(fem OsqpEigen::OsqpEigen)
target_link_libraries(fem SuiteSparse::CHOLMOD)
target_link_libraries(fem libmeshviewer)
target_link_libraries(fem jsoncpp)
target_compile_definitions(fem PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Resource")

#message("Success!")
