#include <memory>
#include "cpu_multiplier.hpp"
#include "matrix_transform/interfaces.hpp"
#include "matrix_transform/factory.hpp"

namespace MatrixTransform {
        
    std::unique_ptr<MatrixTransform::IMultiplier> Factory::createMultiplier(const std::string& type) {
        if (type == "cpu") {
            return std::make_unique<CPUMultiplier>();
        } else {
            throw std::invalid_argument("Unknown multiplier type: " + type);
        }
    }

} // namespace MatrixTransform