# ============================
# || Project Configuration: ||
# ============================

cmake_minimum_required(VERSION 3.20)
project(OptiLayer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(.)


# ================
# || Libraries: ||
# ================

add_library(stl stl/stl.cpp)

add_library(mesh mesh/mesh.cpp)
target_link_libraries(mesh stl)


# ==================
# || Executables: ||
# ==================

add_executable(optilayer optilayer/optilayer.cpp)
target_link_libraries(optilayer stl mesh)

add_executable(scratch scratch.cpp)
target_link_libraries(scratch stl mesh)
