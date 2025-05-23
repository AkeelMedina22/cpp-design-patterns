#include "cpu_multiplier.hpp"
#include <Eigen/Dense>

namespace MatrixTransform {
    
    Matrix CpuMultiplier::multiply(const Matrix& a, const Matrix& b) {
        if (a.cols() != b.rows()) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        return a * b;
    }

} // namespace MatrixTransform