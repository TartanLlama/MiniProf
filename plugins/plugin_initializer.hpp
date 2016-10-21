#pragma once

#include "data_providers/most_called/most_called.hpp"

void initialize_plugins() {
    // Process launchers
    
    
    // Data providers
    miniprof::most_called::register_provider();
}
