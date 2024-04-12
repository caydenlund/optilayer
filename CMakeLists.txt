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


# ==================
# || Executables: ||
# ==================

add_executable(optilayer optilayer/optilayer.cpp)
