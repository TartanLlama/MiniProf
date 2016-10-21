#pragma once

#include "target.hpp"

namespace miniprof {
    class data_provider {
    public:
        virtual ~data_provider() = default;
        
        virtual void new_sample (const process::thread_collection& threads) = 0;
        virtual std::string get_json() = 0;
    };
}
