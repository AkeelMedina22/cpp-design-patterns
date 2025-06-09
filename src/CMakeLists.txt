add_library(matrix_core SHARED)

target_sources(matrix_core PRIVATE
    patterns/factory.cpp
    patterns/multiplier_registry.cpp
    utils/logger.cpp
    multipliers/cpu_naive_multiplier.cpp
)
target_include_directories(matrix_core PUBLIC ...)
target_link_libraries(matrix_core PUBLIC eigen NlohmannJson)


if(ENABLE_OPENMP)
    message(STATUS "Configuring OpenMP backend for matrix_core")
    target_sources(matrix_core PRIVATE multipliers/openmp_multiplier.cpp)
    target_compile_definitions(matrix_core PUBLIC WITH_OPENMP)
    target_link_libraries(matrix_core PUBLIC OpenMP::OpenMP_CXX) 
endif()


if(ENABLE_CUDA)
    message(STATUS "Configuring CUDA backends for matrix_core")
    target_sources(matrix_core PRIVATE
        multipliers/cuda_naive_multiplier.cu
        multipliers/cublas_multiplier.cpp
    )
    target_compile_definitions(matrix_core PUBLIC WITH_CUDA)
    target_link_libraries(matrix_core PUBLIC CUDA::cudart CUDA::cublas) 
    set_target_properties(matrix_core PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
endif()


if(VECTORIZATION_MODE STREQUAL "AVX2")
    message(STATUS "Configuring AVX2 backend for matrix_core")
    target_sources(matrix_core PRIVATE multipliers/avx2_multiplier.cpp)
    target_compile_options(matrix_core PRIVATE -mavx2)
    target_compile_definitions(matrix_core PUBLIC WITH_AVX2)
endif()


add_executable(matrix_app main.cpp)
target_link_libraries(matrix_app PRIVATE matrix_core)