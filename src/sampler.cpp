 // Copyright (C) 2002-2016 Codeplay Software Limited. All Rights Reserved.

#include "sampler.hpp"

uint32_t miniprof::process_launcher::s_current_plugin_id = 0;

std::unordered_map<std::string, std::function<std::unique_ptr<miniprof::process_launcher>()>>
    miniprof::process_launcher::s_plugin_map;
