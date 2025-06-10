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

        float sum = 0.0f;

        for (int tile_num = 0; tile_num < (colsA + TILE_WIDTH - 1) / TILE_WIDTH; ++tile_num) {

            int source_A_row = row;
            int source_A_col = tile_num * TILE_WIDTH + threadIdx.x;

            int source_B_row = tile_num * TILE_WIDTH + threadIdx.y;
            int source_B_col = col;

            if (source_A_row < rowsA && source_A_col < colsA) {
                tile_A[threadIdx.y][threadIdx.x] = d_A[source_A_col * rowsA + source_A_row];
            } else {
                tile_A[threadIdx.y][threadIdx.x] = 0.0f;
            }

            if (source_B_row < colsA && source_B_col < colsB) {
                tile_B[threadIdx.y][threadIdx.x] = d_B[source_B_col * colsA + source_B_row];
            } else {
                tile_B[threadIdx.y][threadIdx.x] = 0.0f;
            }

            __syncthreads();

            for (int inner_k = 0; inner_k < TILE_WIDTH; ++inner_k) {
                sum += tile_A[threadIdx.y][inner_k] * tile_B[inner_k][threadIdx.x];
            }

            __syncthreads();
        }

        if (row < rowsA && col < colsB) {
            d_C[col * rowsA + row] = sum;
        }
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
        matmul_kernel<TILE_WIDTH><<<numBlocks, threadsPerBlock>>>(d_C, d_A, d_B, a.rows(), a.cols(), b.cols());

        gpuErrchk(cudaGetLastError());
        gpuErrchk(cudaDeviceSynchronize()); 

        Logger::getInstance().log(LogLevel::Debug, "Copying result matrix from GPU...");
        gpuErrchk(cudaMemcpy(c.data(), d_C, d_C_bytes, cudaMemcpyDeviceToHost));

        gpuErrchk(cudaDeviceSynchronize());

        Logger::getInstance().log(LogLevel::Debug, "Freeing GPU memory...");
        gpuErrchk(cudaFree(d_A));
        gpuErrchk(cudaFree(d_B));
        gpuErrchk(cudaFree(d_C));

        Logger::getInstance().log(LogLevel::Info, "CUDA multiplication complete.");
        return c;
    }

} // namespace MatrixTransform

// #endif