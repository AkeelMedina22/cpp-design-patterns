#define EIGEN_DONT_PARALLELIZE
#define EIGEN_DONT_VECTORIZE

#include <Eigen/Dense>
#include "logger.hpp"
#include "cpu_multiplier.hpp"
#include "multiplier_registry.hpp"
#include <chrono>

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
        
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        Matrix c = a * b;
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto tm_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        Logger::getInstance().log(LogLevel::Info, "CPU multiplication complete in -> " + std::to_string(tm_duration) + " milliseconds.");
        return c;
    }
    
} // namespace MatrixTransform