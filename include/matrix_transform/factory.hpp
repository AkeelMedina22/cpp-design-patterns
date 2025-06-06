#pragma once
#include <memory>
#include "interfaces.hpp"

namespace MatrixTransform {

    class Factory {
    public:
        Factory() = delete;
        Factory(const Factory&) = delete;
        Factory& operator=(const Factory&) = delete;
        
        static std::unique_ptr<IMultiplier> createMultiplier();
        static std::unique_ptr<IMultiplier> createMultiplier(const std::string& configFilePath);
    };

} // namespace MatrixTransform