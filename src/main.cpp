#include <iostream>
#include "matrix_transform/matrix_types.hpp"
#include "matrix_core/cpu_multiplier.cpp"

int main(int argc, char const *argv[])
{
    MatrixTransform::CpuMultiplier multiplier;
    Matrix a(2, 2);
    Matrix b(2, 2);
    
    a << 1, 2,
         3, 4;
    
    b << 5, 6,
         7, 8;
    
    try {
        Matrix result = multiplier.multiply(a, b);
        std::cout << "Result of multiplication:\n" << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}