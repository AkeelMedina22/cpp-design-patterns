# C++ Design Patterns: Matrix Multiplication Library

A matrix multiplication library written in C++. This project serves as a learning exercise for implementing and understanding common C++ design patterns, primarily:
* Factory Pattern
* Registry Pattern
* Meyers' Singleton Pattern

**Key Feature:** New **multiplier implementations** (e.g., single-threaded CPU, CUDA-accelerated GPU, OpenCL) can be registered and used via the factory without requiring modifications to the factory itself.

**Current Status:**
* Includes a single-threaded CPU multiplier implementation using Eigen.
* (Future: CUDA and OpenCL implementations planned)
* (Future: Add a config to each multiplier instance) 

**Dependencies:**
* C++11+ (for thread-safe static variables)
* CMake 
* Eigen (included as a submodule)

## Building and Running

1.  **Configure with CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

2.  **Build the project:**
    ```bash
    make
    ```

3.  **Run the example application:**
    ```bash
    src/matrix_app 
    ```

## How to Use 
```cpp
#include "matrix_transform/matrix_types.hpp"
#include "matrix_transform/interfaces.hpp"
#include "matrix_transform/factory.hpp"

using MatrixTransform::Factory;
// ...
auto multiplier = Factory::createMultiplier("CPU");
Matrix a = /* ... */;
Matrix b = /* ... */;
Matrix result = multiplier->multiply(a, b);
// ...
