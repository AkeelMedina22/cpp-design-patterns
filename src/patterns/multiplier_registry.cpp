#include "multiplier_registry.hpp"

namespace MatrixTransform {

    MultiplierRegistry& MultiplierRegistry::getInstance(){
        static MultiplierRegistry instance;
        return instance;
    }
    
    std::unique_ptr<IMultiplier> MultiplierRegistry::createMultiplier(const std::string& key){
        auto it = creatorMap_.find(key);
        if (it != creatorMap_.end()) {
            return it->second(); 
        }
        throw std::runtime_error("MultiplierRegistry: Unknown multiplier type requested: " + key);
    }

    bool MultiplierRegistry::registerMultiplier(const std::string& key, std::function<std::unique_ptr<IMultiplier>()> creator){
        auto result = creatorMap_.emplace(key, std::move(creator));
        if (!result.second) {
            creatorMap_[key] = std::move(result.first->second); 
        }
        return true;
    } 
}