#ifdef WITH_CUDA 

#include "multiplier_registry.hpp"
#include "cublas_multiplier.hpp" 
#include "logger.hpp"
#include <cuda_runtime.h>
#include <cublas_v2.h> 
#include <iostream>
#include <memory>

namespace MatrixTransform {

    namespace { 
        MultiplierRegistrar CublasRegistrar( 
            "cuBLAS", 
            []() -> std::unique_ptr<IMultiplier> { 
                return std::make_unique<cuBLASMultiplier>();
            }
        );
    }

    #define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
    inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true){
        if (code != cudaSuccess){
            fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
            if (abort) exit(code);
        }
    }

    #define cuBLASErrchk(ans) { cuBLASAssert((ans), __FILE__, __LINE__); }
    inline void cuBLASAssert(cublasStatus_t code, const char *file, int line, bool abort=true){
        if (code != CUBLAS_STATUS_SUCCESS){
            fprintf(stderr,"cuBLASassert: %s %d\n", file, line);
            if (abort) exit(code);
        }
    }

    Matrix cuBLASMultiplier::multiply(const Matrix& a, const Matrix& b) {
        Logger::getInstance().log(LogLevel::Debug, "Starting cuBLAS multiplication.");

        if (a.cols() != b.rows()) {
            Logger::getInstance().log(LogLevel::Error, "Matrix dimension mismatch.");
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }

        cublasHandle_t handle;
        cuBLASErrchk(cublasCreate(&handle));

        float *d_A, *d_B, *d_C; 
        size_t d_A_bytes = a.rows() * a.cols() * sizeof(float);
        size_t d_B_bytes = b.rows() * b.cols() * sizeof(float);
        size_t d_C_bytes = a.rows() * b.cols() * sizeof(float);
        
        Logger::getInstance().log(LogLevel::Debug, "Allocating memory on GPU...");
        gpuErrchk(cudaMalloc(&d_A, d_A_bytes));
        gpuErrchk(cudaMalloc(&d_B, d_B_bytes));
        gpuErrchk(cudaMalloc(&d_C, d_C_bytes));

        Logger::getInstance().log(LogLevel::Debug, "Copying matrices to GPU...");
        cuBLASErrchk(cublasSetMatrix(a.rows(), a.cols(), sizeof(float), a.data(), a.rows(), d_A, a.rows()));
        cuBLASErrchk(cublasSetMatrix(b.rows(), b.cols(), sizeof(float), b.data(), b.rows(), d_B, b.rows()));
        
        Logger::getInstance().log(LogLevel::Debug, "Calling cublasSgemm...");
        
        const float alpha = 1.0f; 
        const float beta = 0.0f; 

        cuBLASErrchk(cublasSgemm(handle,
                                 CUBLAS_OP_N, // Operation on A: No transpose
                                 CUBLAS_OP_N, // Operation on B: No transpose
                                 a.rows(),    // Rows of A (and C)
                                 b.cols(),    // Columns of B (and C)
                                 a.cols(),    // Columns of A (and rows of B)
                                 &alpha,      // Pointer to alpha
                                 d_A,         // Pointer to A on device
                                 a.rows(),    // Leading dimension of A (lda)
                                 d_B,         // Pointer to B on device
                                 b.rows(),    // Leading dimension of B (ldb)
                                 &beta,       // Pointer to beta
                                 d_C,         // Pointer to C on device
                                 a.rows()     // Leading dimension of C (ldc)
                                 ));

        Matrix c(a.rows(), b.cols());
        Logger::getInstance().log(LogLevel::Debug, "Copying result from GPU...");
        cuBLASErrchk(cublasGetMatrix(c.rows(), c.cols(), sizeof(float), d_C, c.rows(), c.data(), c.rows()));

        Logger::getInstance().log(LogLevel::Debug, "Freeing GPU memory and cuBLAS handle...");
        gpuErrchk(cudaFree(d_A));
        gpuErrchk(cudaFree(d_B));
        gpuErrchk(cudaFree(d_C));
        cuBLASErrchk(cublasDestroy(handle));

        Logger::getInstance().log(LogLevel::Info, "cuBLAS multiplication complete.");
        return c;
    }

} // namespace MatrixTransform

#endif 
