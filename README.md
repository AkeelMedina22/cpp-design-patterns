# C++ Design Patterns: Matrix Multiplication Library

A matrix multiplication library written in C++. This project serves as a learning exercise for implementing and understanding common C++ design patterns, primarily:
* Factory Pattern
* Registry Pattern
* Meyers' Singleton Pattern

**Key Feature:** New **multiplier implementations** (e.g., single-threaded CPU, CUDA-accelerated GPU, OpenCL) can be registered and used via the factory without requiring modifications to the factory itself. Each multiplier is generated using a config, and such, it may be useful, for example, in performance testing multiple algorithms/hyperparameters.

**Current Status:**
* Includes a registry that works as follows:
    - Static initialization order MUST create the `CPURegistrar` object defined in `*_multiplier.cpp` before main.
    - The constructor of `MultiplierRegistrar` initializes the `MultiplierRegistry` using `getInstance` (Meyers Singleton Pattern).
    - All `IMultiplier` implementations will thus have their 'recipe' stored in the Registry before main.
    - By default, a Registration of the same name will be overwritten.
    - Move semantics are utilized for the efficient transfer of ownership of unique_ptr from Multiplier to Registry. 
* Includes a single-threaded CPU multiplier implementation using Eigen.
* Includes a globally accessible Logger singleton for handling application-wide logging.
* All backends are created and configured via a central JSON config file.
* (Future: CUDA and OpenMP implementations planned).

**Dependencies:**
* C++17 (for thread-safe static variables, move semantics, smart pointers, and `try_emplace`)
* CMake 3.22.1 or higher
* Eigen (included as a git submodule)
* Json (included as a git submodule)

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
    src/matrix_app ../src/config_cpu.json
    ```

Alternatively, you can simply run `make` to use the default configuration.

## How to Use 
```cpp
#include "matrix_transform/matrix_types.hpp"
#include "matrix_transform/interfaces.hpp"
#include "matrix_transform/factory.hpp"

using MatrixTransform::Factory;
// ...
auto multiplier = Factory::createMultiplier("../src/config.json");
Matrix a = /* ... */;
Matrix b = /* ... */;
Matrix result = multiplier->multiply(a, b);
// ...
