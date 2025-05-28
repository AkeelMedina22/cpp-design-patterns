#include <Eigen/Dense>
#include "cpu_multiplier.hpp"
#include "multiplier_registry.hpp"

namespace MatrixTransform {

    namespace { 
        MultiplierRegistrar CPURegistrar( 
            "CPU",
            []() -> std::unique_ptr<IMultiplier> { 
                return std::make_unique<CPUMultiplier>();
            }
        );
    }
    
    Matrix CPUMultiplier::multiply(const Matrix& a, const Matrix& b) {
        if (a.cols() != b.rows()) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        return a * b;
    }

} // namespace MatrixTransform