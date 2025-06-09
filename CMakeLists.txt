cmake_minimum_required(VERSION 3.22.1)
project(mmult_design VERSION 1.0 LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_OPENMP "Build with OpenMP support" ON)
option(ENABLE_CUDA "Build with CUDA support" OFF)

set(VECTORIZATION_MODE "AUTO" CACHE STRING "SIMD vectorization mode (AUTO, AVX2, SSE4)")

if(ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
    message(STATUS "OpenMP backend enabled.")
endif()

if(ENABLE_CUDA)
    find_package(CUDAToolkit REQUIRED)
    message(STATUS "CUDA backend enabled.")
endif()

add_subdirectory(extern/eigen)
add_subdirectory(src)