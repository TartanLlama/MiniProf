// Copyright (C) 2002-2016 Codeplay Software Limited. All Rights Reserved.

#pragma once

#include <chrono>
#include <thread>
#include <vector>
#include "target.hpp"

static constexpr std::chrono::nanoseconds default_sample_rate = std::chrono::nanoseconds(10000000000);


namespace miniprof {
    class sampler {
    public:
        sampler (process& process, std::chrono::nanoseconds sample_rate = default_sample_rate)
            : m_process{process}, m_sample_rate{sample_rate} {}
        
        void set_sample_rate (std::chrono::milliseconds ms) {
            m_sample_rate = std::chrono::duration_cast<std::chrono::nanoseconds>(ms);
        }
        
        void set_sample_rate (std::chrono::microseconds ms) {
           m_sample_rate = std::chrono::duration_cast<std::chrono::nanoseconds>(ms);
        }
        
        void set_sample_rate (std::chrono::nanoseconds ns) { m_sample_rate = ns; }

        std::vector<std::unique_ptr<thread>> sample() {
            m_process.start();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            m_process.halt();
            return m_process.get_threads();
        }
        
    private:
        std::chrono::nanoseconds m_sample_rate;
        process& m_process;
    };
}
