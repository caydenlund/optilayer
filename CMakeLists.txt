# ============================
# || Project Configuration: ||
# ============================

cmake_minimum_required(VERSION 3.20)
project(OptiLayer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)


# ================
# || Libraries: ||
# ================

# add_library(mesh)

add_library(stl stl/stl.cpp)


# ==================
# || Executables: ||
# ==================

# add_executable(optilayer optilayer/optilayer.cpp)
# target_link_libraries(optilayer mesh stl)

add_executable(scratch scratch.cpp)
target_link_libraries(scratch stl)

