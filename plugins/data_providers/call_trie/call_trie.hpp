#pragma once

#include "data_provider.hpp"
#include "target.hpp"

namespace miniprof {
    class node {
    public:
        void add_nodes (std::list<std::string>& list) {
            ++m_value;

            if (!list.empty()) {
                auto back = list.back();
                list.pop_back();
                m_children[back]->add_nodes(list);
            }
        }

    private:
        std::size_t m_value;
        std::unordered_map<std::string, std::unique_ptr<node>> m_children;
    };
    
    class call_trie : public data_provider {
    public:
        static void register_provider();

        void new_sample (const process::thread_collection& threads) override;
        std::string get_json() override;

    private:
        node m_trie;
    };
}
