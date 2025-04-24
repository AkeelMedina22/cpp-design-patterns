cmake_minimum_required(VERSION 3.22.1)
project(mmult_design VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(extern/eigen)
target_include_directories(include)

add_subdirectory(src)
