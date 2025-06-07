#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "logger.hpp"
#include "cpu_multiplier.hpp"
#include "multiplier_registry.hpp"
#include "matrix_transform/interfaces.hpp"
#include "matrix_transform/factory.hpp"


namespace MatrixTransform {

    std::unique_ptr<MatrixTransform::IMultiplier> Factory::createMultiplier() {
        return MultiplierRegistry::getInstance().createMultiplier("CPU");
    }
        
    std::unique_ptr<MatrixTransform::IMultiplier> Factory::createMultiplier(const std::string& configFilePath) {
        
        std::ifstream configFileStream(configFilePath);
        if (!configFileStream.is_open()) {
            throw std::runtime_error("Could not open config file: " + configFilePath);
        }

        nlohmann::json configJson;
        std::unique_ptr<MatrixTransform::IMultiplier> multiplier;

        try {
            configFileStream >> configJson;

            std::string logLevelStr = configJson.value("log_level", "info");
            Logger::getInstance().setLevel(logLevelStr);

            if (configJson.is_null()) {
                Logger::getInstance().log(LogLevel::Error, "Failed to parse JSON from " + configFilePath);
                throw std::runtime_error("Config file is empty or contains only null.");
            }

            std::string multiplierType = configJson.value("backend", "CPU");
            Logger::getInstance().log(LogLevel::Info, "Creating backend: " + multiplierType);

            multiplier = MultiplierRegistry::getInstance().createMultiplier(multiplierType);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error parsing JSON from " + configFilePath + ": " + e.what());
        }

        return multiplier;
    }

} // namespace MatrixTransform