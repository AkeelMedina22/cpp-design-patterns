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

add_library(NlohmannJson INTERFACE) 
target_include_directories(NlohmannJson INTERFACE
    "${PROJECT_SOURCE_DIR}/extern/nlohmann_json/single_include" 
)

target_link_libraries(matrix_core
    PUBLIC
        eigen
        NlohmannJson
)

add_executable(matrix_app main.cpp)
target_link_libraries(matrix_app PRIVATE matrix_core)