add_library(matrix_core SHARED)

target_sources(matrix_core PRIVATE
    patterns/factory.cpp
    patterns/multiplier_registry.cpp
    utils/logger.cpp
    matrix_core/cpu_multiplier.cpp
)
target_include_directories(matrix_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        matrix_core
        patterns
        utils
)
add_library(NlohmannJson INTERFACE) 
target_include_directories(NlohmannJson INTERFACE
    "${PROJECT_SOURCE_DIR}/extern/nlohmann_json/single_include" 
)
target_link_libraries(matrix_core PUBLIC eigen NlohmannJson)

if(ENABLE_CUDA)
    message(STATUS "Configuring CUDA backends for matrix_core")
    target_sources(matrix_core PRIVATE
        matrix_core/cuda_multiplier.cu
        matrix_core/cublas_multiplier.cpp
    )
    target_compile_definitions(matrix_core PUBLIC WITH_CUDA)
    target_link_libraries(matrix_core PUBLIC CUDA::cudart CUDA::cublas) 
    set_target_properties(matrix_core PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
endif()


if(VECTORIZATION_MODE STREQUAL "AVX2" AND ENABLE_OPENMP)
    message(STATUS "Configuring OMP+AVX2 backend for matrix_core")
    target_sources(matrix_core PRIVATE matrix_core/ompavx2_multiplier.cpp)
    target_compile_definitions(matrix_core PUBLIC WITH_OMPAVX2)
    target_link_libraries(matrix_core PUBLIC OpenMP::OpenMP_CXX)
endif()


if(VECTORIZATION_MODE STREQUAL "NEON")
    message(STATUS "Configuring NEON backend for matrix_core")
    target_sources(matrix_core PRIVATE matrix_core/neon_multiplier.cpp)
    target_compile_definitions(matrix_core PUBLIC WITH_NEON)
endif()


add_executable(matrix_app main.cpp)
target_link_libraries(matrix_app PRIVATE matrix_core)