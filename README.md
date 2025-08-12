# C++ Design Patterns: Matrix Multiplication Library

A matrix multiplication library written in C++. This project serves as a learning exercise for implementing and understanding common C++ design patterns, primarily:
* Factory Pattern
* Registry Pattern
* Meyers' Singleton Pattern

**Key Feature:** New **multiplier implementations** (e.g., single-threaded CPU, CUDA-accelerated GPU, OpenCL) can be registered and used via the factory without requiring modifications to the factory itself. Each multiplier is generated using a config, and such, it may be useful, for example, in performance testing multiple algorithms/hyperparameters.

Read more about this project on my [blog](https://akeelmedina22.github.io/posts/2025-07/cpp-design-patterns/)!

**Current Status:**
* Includes a registry that contains all Multiplier implementations.
* Includes a single-threaded CPU multiplier implementation using Eigen.
* Includes CUDA and OpenMP implementations for comparison (with SIMD-> AVX2+NEON).
* Includes a globally accessible Logger singleton for handling application-wide logging.
* All backends are created and configured via a central JSON config file.

**Dependencies:**
* C++17 (for thread-safe static variables, move semantics, smart pointers, and `try_emplace`)
* CMake 3.22.1 or higher
* Eigen (included as a git submodule)
* Json (included as a git submodule)

## Building and Running

1. **Clone the Repository:**
    ```bash
    git clone git@github.com:AkeelMedina22/cpp-design-patterns.git
    git submodule update --init --recursive
    ```
    
2.  **Configure with CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Build the project:**
    ```bash
    make
    ```

4.  **Run the example application:**
    ```bash
    src/matrix_app ../src/config.json
    ```

## How to Use 
```cpp
#include <memory>
#include <iostream>
#include <string>
#include "matrix_transform/matrix_types.hpp"
#include "matrix_transform/interfaces.hpp"
#include "matrix_transform/factory.hpp"

using MatrixTransform::Factory;
// ...
str::string configFilePath = argv[1]; 
auto multiplier = Factory::createMultiplier(configFilePath);
Matrix a = /* ... */;
Matrix b = /* ... */;
Matrix result = multiplier->multiply(a, b);
// ...
