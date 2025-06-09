#include <memory>
#include <iostream>
#include <string>
#include "matrix_transform/matrix_types.hpp"
#include "matrix_transform/interfaces.hpp"
#include "matrix_transform/factory.hpp"

using MatrixTransform::Factory;

int main(int argc, char const *argv[])
{
    std::string configFilePath;

    if (argc > 1) {
        configFilePath = argv[1]; 
        std::cout << "Config file path provided: " << configFilePath << std::endl;
    } else {
        std::cerr << "Usage: " << argv[0] << " <path_to_config.json>" << std::endl;
        std::cerr << "No config file path provided. Exiting." << std::endl;
        return 1; 
    }

    std::unique_ptr<MatrixTransform::IMultiplier> multiplier;
    multiplier = Factory::createMultiplier(configFilePath);

    Matrix a(2, 2);
    Matrix b(2, 2);
    
    a << 2, 2,
         3, 4;
    
    b << 5, 6,
         7, 8;
    
    try {
        Matrix result = multiplier->multiply(a, b);
        std::cout << "Result of multiplication:\n" << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}