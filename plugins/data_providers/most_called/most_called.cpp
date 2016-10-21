#include <sstream>
#include <iterator>
#include <mutex>

#include "most_called.hpp"
#include "data_provider_registry.hpp"

using namespace miniprof;

void most_called::register_provider() {
    data_provider_registry::register_data_provider("most_called", std::make_unique<most_called>());
}

std::string most_called::get_json() {
    std::lock_guard<std::mutex> guard {m_mutex};

    std::ostringstream json;
    json << '[';


    auto it = m_call_counts.begin();
    for (std::size_t i=0; i < m_top_count && it != m_call_counts.end(); ++i, ++it) {
        json << "[\"" << it->first << "\"," << it->second << ']';

        if (std::next(it) != m_call_counts.end() && i < m_top_count - 1) {
            json << ',';
        }
    }

    json << ']';

    return json.str();
}

void most_called::new_sample (const process::thread_collection& threads) {
    std::lock_guard<std::mutex> guard {m_mutex};
    
    if (!threads.empty()) {
        auto frames = threads[0]->get_frames();
//        for (auto&& frame : frames) {
        if (!frames.empty()) {
            auto func = frames[0]->get_function();

            if (m_index.count(func.get_start_address())) {
                auto it = m_index[func.get_start_address()];

                //increment the hit count
                ++it->second;

                //sort if necessary
                for (auto prev = std::prev(it);
                     it != m_call_counts.begin() && it->second > prev->second;
                     prev = std::prev(it)) {
                    std::swap(*it, *prev);
                    it = prev;
                }
            }
            else {
                m_call_counts.emplace_back(func.get_name(), 0);
                m_index[func.get_start_address()] = std::prev(m_call_counts.end());
            }
        }
    }
}
