target_sources(matrix_core PRIVATE
    matrix_core/cpu_multiplier.cpp
    matrix_core/factories.cpp
    matrix_core/multiplier_registry.cpp
)

target_include_directories(matrix_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        . 
)

target_link_libraries(matrix_core
    PUBLIC
        Eigen3::eigen
)