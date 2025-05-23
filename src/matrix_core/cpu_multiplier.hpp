#pragma once
#include "matrix_transform/interfaces.hpp" 

namespace MatrixTransform {

    class CpuMultiplier : public IMultiplier {
    public:
        Matrix multiply(const Matrix& a, const Matrix& b) override;
    };

} // namespace MatrixTransform