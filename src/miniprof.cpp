// Copyright (C) 2002-2016 Codeplay Software Limited. All Rights Reserved.

#include "target.hpp"
#include "data_provider_registry.hpp"
#include "sampler.hpp"
#include "server.hpp"
#include "../plugins/process_launchers/lldb/lldb_target.hpp"
#include "../plugins/plugin_initializer.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace miniprof;

int main() {
    initialize_plugins();
    lldb_process_launcher{};
    auto launcher = miniprof::process_launcher::get_process_launcher_plugin("lldb");

    auto process = launcher->launch("/home/simon/work/MiniProf/build/a.out", "");

    data_server server{};
    server.run();
    
    while (!process->is_finished()) {
        sampler sampler {*process};
        auto threads = sampler.sample();
        std::cerr << threads.size();
        for (const auto& provider : data_provider_registry::get_providers()) {
            provider.second->new_sample(threads);
        }
    }
}
