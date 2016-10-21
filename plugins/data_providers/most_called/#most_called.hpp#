#pragma once

#include <list>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <string>

#include "data_provider.hpp"
#include "target.hpp"

namespace miniprof {
    class most_called : public data_provider {
    public:
        static void register_provider();
        
        most_called (std::size_t top_count=s_default_top_count) : m_top_count{top_count} {}
        void new_sample (const process::thread_collection& threads) override;
        std::string get_json() override;

    private:
        std::size_t m_top_count;
        
        std::list<std::pair<std::string, std::size_t>> m_call_counts;
        std::unordered_map<uint64_t, decltype(m_call_counts)::iterator> m_index;
        std::mutex m_mutex;

        static constexpr std::size_t s_default_top_count = 10;
    };
}

