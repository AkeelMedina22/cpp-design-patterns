#pragma once
#include "matrix_transform/interfaces.hpp" 

namespace MatrixTransform {

    class NEONMultiplier : public IMultiplier {
    public:
        Matrix multiply(const Matrix& a, const Matrix& b) override;
    };

} // namespace MatrixTransform