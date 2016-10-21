#include "data_provider_registry.hpp"
#include "data_provider.hpp"

using namespace miniprof;
std::unordered_map<std::string, std::unique_ptr<data_provider>> data_provider_registry::s_providers;
