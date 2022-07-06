cmake_minimum_required(VERSION 3.20)
project(Renderer)

if (DEBUG)
        set(CMAKE_BUILD_TYPE "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
else()
        set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(spdlog)

include(cmake/get_cpm.cmake)

set(glad_SOURCE_DIR 3rdParty/glad)
add_subdirectory(${glad_SOURCE_DIR})
include_directories(${glad_SOURCE_DIR}/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")

CPMAddPackage(
        NAME glfw
        GIT_REPOSITORY https://mirror.ghproxy.com/https://github.com/glfw/glfw.git
        GIT_TAG 45ce5ddd197d5c58f50fdd3296a5131c894e5527
)
include_directories(${glfw_SOURCE_DIR}/include)

CPMAddPackage(
        NAME eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 67eeba6e720c5745abc77ae6c92ce0a44aa7b7ae
)
include_directories(${eigen_SOURCE_DIR}/include)

CPMAddPackage(
        NAME glm
        GIT_REPOSITORY https://github.com/Groovounet/glm.git
        GIT_TAG e1afbc9ceaacbc9aed7bb896d26c0872f8a2bf29
        PATCH_COMMAND git am ${CMAKE_SOURCE_DIR}/patch/glm.patch
)
include_directories(${glm_SOURCE_DIR})

include_directories(src)

file(GLOB SOURCE_FILES
        ./src/Camera/*.cc
        ./src/GUI/*.cc
        ./src/Light/*.cc
        ./src/Material/*.cc
        ./src/MeshObject/*.cc
        ./src/Shader/*.cc)

add_library(libmeshviewer ${SOURCE_FILES})
target_link_libraries(libmeshviewer OpenGL::GL)
target_link_libraries(libmeshviewer glfw)
target_link_libraries(libmeshviewer glad)
target_link_libraries(libmeshviewer spdlog::spdlog)
target_link_libraries(libmeshviewer Eigen3::Eigen)

if (BUILD_EXAMPLE_MESH_VIEWER)
        add_executable(main src/Example/main.cc src/Example/ExampleApp.cc)
        target_link_libraries(main libmeshviewer)
endif()