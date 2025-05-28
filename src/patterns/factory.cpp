#include <memory>
#include "cpu_multiplier.hpp"
#include "multiplier_registry.hpp"
#include "matrix_transform/interfaces.hpp"
#include "matrix_transform/factory.hpp"


namespace MatrixTransform {
        
    std::unique_ptr<MatrixTransform::IMultiplier> Factory::createMultiplier(const std::string& type) {
        return MultiplierRegistry::getInstance().createMultiplier(type);
    }

} // namespace MatrixTransform