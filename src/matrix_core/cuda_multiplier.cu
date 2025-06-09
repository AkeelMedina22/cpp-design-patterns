// #ifdef WITH_CUDA

#include <Eigen/Dense>
#include <iostream>
#include <cuda_runtime.h>
#include "logger.hpp"
#include "cuda_multiplier.hpp"
#include "multiplier_registry.hpp"

namespace MatrixTransform {

    namespace { 
        MultiplierRegistrar CUDARegistrar( 
            "CUDA",
            []() -> std::unique_ptr<IMultiplier> { 
                return std::make_unique<CUDAMultiplier>();
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

    template <int TILE_WIDTH>
    __global__ void matmul_kernel(float* d_C, const float* d_A, const float* d_B, int rowsA, int colsA, int colsB) {
        
        int row = blockIdx.y * TILE_WIDTH + threadIdx.y;
        int col = blockIdx.x * TILE_WIDTH + threadIdx.x;

        __shared__ float tile_A[TILE_WIDTH][TILE_WIDTH];
        __shared__ float tile_B[TILE_WIDTH][TILE_WIDTH];

        float sum;

        for (int k = 0; k < colsA / TILE_WIDTH; ++k){
            int Ax = blockIdx.y * TILE_WIDTH + threadIdx.y;
            int Ay = k * TILE_WIDTH + threadIdx.x;

            int Bx = k * TILE_WIDTH + threadIdx.y;
            int By = blockIdx.x * TILE_WIDTH + threadIdx.x;

            tile_A[threadIdx.y][threadIdx.x] = d_A[Ax * colsA + Ay];
            tile_B[threadIdx.y][threadIdx.x] = d_B[Bx * colsB + By];

            __syncthreads();

            for (int k = 0; k < TILE_WIDTH; ++k) {
                sum += tile_A[threadIdx.y][k] * tile_B[k][threadIdx.x];
            }

            __syncthreads();
        }

        d_C[row * colsB + col] = sum;
    }
        
    Matrix CUDAMultiplier::multiply(const Matrix& a, const Matrix& b) {
        Logger::getInstance().log(LogLevel::Debug, "Starting CUDA multiplication.");

        if (a.cols() != b.rows()) {
            Logger::getInstance().log(LogLevel::Error, "Matrix dimension mismatch.");
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }

        Matrix c(a.rows(), b.cols());

        float* d_A, *d_B, *d_C; 
        size_t d_A_bytes = a.rows() * a.cols() * sizeof(float);
        size_t d_B_bytes = b.rows() * b.cols() * sizeof(float);
        size_t d_C_bytes = c.rows() * c.cols() * sizeof(float);
        
        Logger::getInstance().log(LogLevel::Debug, "Allocating memory on GPU...");
        gpuErrchk(cudaMalloc(&d_A, d_A_bytes));
        gpuErrchk(cudaMalloc(&d_B, d_B_bytes));
        gpuErrchk(cudaMalloc(&d_C, d_C_bytes));

        Logger::getInstance().log(LogLevel::Debug, "Copying memory to GPU...");
        gpuErrchk(cudaMemcpy(d_A, a.data(), d_A_bytes, cudaMemcpyHostToDevice));
        gpuErrchk(cudaMemcpy(d_B, b.data(), d_B_bytes, cudaMemcpyHostToDevice));

        Logger::getInstance().log(LogLevel::Debug, "Zeroing out device result matrix C...");
        gpuErrchk(cudaMemset(d_C, 0, d_C_bytes));

        constexpr int TILE_WIDTH = 16;
        dim3 threadsPerBlock(TILE_WIDTH, TILE_WIDTH);
        dim3 numBlocks( (c.cols() + threadsPerBlock.x - 1) / threadsPerBlock.x,
                        (c.rows() + threadsPerBlock.y - 1) / threadsPerBlock.y );

        Logger::getInstance().log(LogLevel::Debug, "Launching matmul kernel...");
        matmul_kernel<32><<<numBlocks, threadsPerBlock>>>(d_C, d_A, d_B, a.rows(), a.cols(), b.cols());

        gpuErrchk(cudaGetLastError());
        gpuErrchk(cudaDeviceSynchronize()); 

        Logger::getInstance().log(LogLevel::Debug, "Copying result matrix from GPU...");
        gpuErrchk(cudaMemcpy(c.data(), d_C, d_C_bytes, cudaMemcpyDeviceToHost));

        Logger::getInstance().log(LogLevel::Debug, "Freeing GPU memory...");
        gpuErrchk(cudaFree(d_A));
        gpuErrchk(cudaFree(d_B));
        gpuErrchk(cudaFree(d_C));

        Logger::getInstance().log(LogLevel::Info, "CUDA multiplication complete.");
        return c;
    }

} // namespace MatrixTransform

// #endif