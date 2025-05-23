#pragma once
#include "matrix_types.hpp"
#include <memory> 

namespace MatrixTransform {

class IMultiplier {
public:
    virtual ~IMultiplier() = default; 
    virtual Matrix multiply(const Matrix& a, const Matrix& b) = 0;
};

} // namespace MatrixTransform