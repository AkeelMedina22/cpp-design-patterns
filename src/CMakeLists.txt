add_library(matrix_core SHARED)

target_sources(matrix_core PRIVATE
    matrix_core/cpu_multiplier.cpp
    patterns/factory.cpp
    patterns/multiplier_registry.cpp
)

target_include_directories(matrix_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        matrix_core
        patterns
)

target_link_libraries(matrix_core
    PUBLIC
        eigen
)

add_executable(matrix_app main.cpp)
target_link_libraries(matrix_app PRIVATE matrix_core)