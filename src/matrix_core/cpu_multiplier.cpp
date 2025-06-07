#include <Eigen/Dense>
#include "logger.hpp"
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
        Logger::getInstance().log(LogLevel::Debug, "Starting CPU multiplication.");

        if (a.cols() != b.rows()) {
            Logger::getInstance().log(LogLevel::Error, "Matrix dimension mismatch.");
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }

        Logger::getInstance().log(LogLevel::Info, "CPU multiplication complete.");
        return a * b;
    }

} // namespace MatrixTransform