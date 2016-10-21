#pragma once

#include "data_provider.hpp"

namespace miniprof {
    class data_provider_registry {
    public:
        static data_provider*
        get_data_provider (const std::string& name) {
            if (s_providers.count(name)) {
                return s_providers[name].get();
            }

            return nullptr;
        }

        static void
        register_data_provider (const std::string& name,
                                std::unique_ptr<data_provider> provider) {
            s_providers[name] = std::move(provider);
        }

        static auto& get_providers() {
            return s_providers;
        }

    private:
        static std::unordered_map<std::string, std::unique_ptr<data_provider>> s_providers;
    };
}
