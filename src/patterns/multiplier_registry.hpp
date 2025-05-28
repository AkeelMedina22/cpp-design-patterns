#include <map>
#include <memory>
#include "matrix_transform/interfaces.hpp"

namespace MatrixTransform {
    class MultiplierRegistry {
        public:
            MultiplierRegistry(const MultiplierRegistry&) = delete;
            MultiplierRegistry& operator=(const MultiplierRegistry&) = delete;

            static MultiplierRegistry& getInstance();
            std::unique_ptr<IMultiplier> createMultiplier(const std::string& key);
            bool registerMultiplier(const std::string& key, std::function<std::unique_ptr<IMultiplier>()> creator);

        private:
            MultiplierRegistry() = default;
            ~MultiplierRegistry() = default;

            std::map<std::string, std::function<std::unique_ptr<IMultiplier>()>> creatorMap_;
    };

    class MultiplierRegistrar {
        public:
            MultiplierRegistrar(const std::string& key, std::function<std::unique_ptr<IMultiplier>()> creator) {
                MultiplierRegistry::getInstance().registerMultiplier(key, std::move(creator));
            }
    };
} // namespace MatrixTransform