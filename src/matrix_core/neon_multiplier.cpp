#ifdef WITH_NEON

#include <Eigen/Dense>
#include "logger.hpp"
#include "neon_multiplier.hpp"
#include "multiplier_registry.hpp"
#include <chrono>
#include <arm_neon.h>

namespace MatrixTransform {

    namespace { 
        MultiplierRegistrar NEONRegistrar( 
            "NEON",
            []() -> std::unique_ptr<IMultiplier> { 
                return std::make_unique<NEONMultiplier>();
            }
        );
    }
    
    Matrix NEONMultiplier::multiply(const Matrix& a, const Matrix& b) {
        Logger::getInstance().log(LogLevel::Debug, "Starting NEON multiplication.");

        if (a.cols() != b.rows()) {
            Logger::getInstance().log(LogLevel::Error, "Matrix dimension mismatch.");
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        Matrix c = Matrix::Zero(a.rows(), b.cols());
        Matrix b_T = b.transpose();
        
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < a.rows(); ++i) {
            for (int j = 0; j < b_T.rows(); ++j) {
                float32x4_t sum_vec = vdupq_n_f32(0.0f);

                int k = 0;
                for (; k <= a.cols() - 4; k += 4) {
                    float32x4_t a_vec = vld1q_f32(&a(i, k));
                    float32x4_t b_vec = vld1q_f32(&b_T(j, k));

                    // Fused multiply-add operation
                    sum_vec = vmlaq_f32(sum_vec, a_vec, b_vec);
                }
                
                // Perform a horizontal sum on the 4-element vector
                float32x2_t sum_pair = vadd_f32(vget_low_f32(sum_vec), vget_high_f32(sum_vec));
                float result = vget_lane_f32(sum_pair, 0) + vget_lane_f32(sum_pair, 1);
                
                // Handle any remaining elements
                for (; k < a.cols(); ++k) {
                    result += a(i, k) * b_T(j, k);
                }
                
                c(i, j) = result;
            }
        }
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto tm_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        Logger::getInstance().log(LogLevel::Info, "NEON multiplication complete in -> " + std::to_string(tm_duration) + " milliseconds.");
        return c;
    }
    
} // namespace MatrixTransform

#endif