#ifdef WITH_OMPAVX2

#include <Eigen/Dense>
#include "logger.hpp"
#include "ompavx2_multiplier.hpp"
#include "multiplier_registry.hpp"
#include <chrono>
#include <omp.h>
#include <immintrin.h> 

namespace MatrixTransform {

    namespace { 
        MultiplierRegistrar OMPAVX2Registrar( 
            "OMP_AVX2",
            []() -> std::unique_ptr<IMultiplier> { 
                return std::make_unique<OMPAVX2Multiplier>();
            }
        );
    }
    
    Matrix OMPAVX2Multiplier::multiply(const Matrix& a, const Matrix& b) {
        Logger::getInstance().log(LogLevel::Debug, "Starting OMP AVX2 multiplication.");

        if (a.cols() != b.rows()) {
            Logger::getInstance().log(LogLevel::Error, "Matrix dimension mismatch.");
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        Matrix c = Matrix::Zero(a.rows(), b.cols());
        Matrix b_T = b.transpose();
        
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        #pragma omp parallel for
        for (int i = 0; i < a.rows(); ++i) {
            for (int j = 0; j < b_T.rows(); ++j) {
                __m256 sum_vec = _mm256_setzero_ps();

                int k = 0;
                for (; k <= a.cols() - 8; k += 8) {
                    __m256 a_vec = _mm256_loadu_ps(&a(i, k));
                    __m256 b_vec = _mm256_loadu_ps(&b_T(j, k));

                    __m256 prod_vec = _mm256_mul_ps(a_vec, b_vec);
                    sum_vec = _mm256_add_ps(sum_vec, prod_vec);
                }

                float sum_arr[8];
                _mm256_storeu_ps(sum_arr, sum_vec);
                float result = sum_arr[0] + sum_arr[1] + sum_arr[2] + sum_arr[3] +
                            sum_arr[4] + sum_arr[5] + sum_arr[6] + sum_arr[7];
                
                for (; k < a.cols(); ++k) {
                    result += a(i, k) * b_T(j, k);
                }
                
                c(i, j) = result;
            }
        }
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto tm_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        Logger::getInstance().log(LogLevel::Info, "OMP AVX2 multiplication complete in -> " + std::to_string(tm_duration) + " milliseconds.");
        return c;
    }
    
} // namespace MatrixTransform

#endif