#inclue "call_trie.hpp"

using namespace miniprof;

void call_trie::new_sample (const process::thread_collection& threads) {
    if (!threads.empty()) {
        std::list<std::string> call_stack;
        auto frames = threads.get_frames();
        std::transform(std::begin(frames), std::end(frames), std::back_inserter(call_stack),
                       [](auto&& e) { return e->get_function().get_name(); });
        m_trie.add_nodes(call_stack);
    }
}

void print_children (std::ostream& str, const std::string& name,
                     const std::unordered_map<std::string, std::unique_ptr<node>>& children) {
    for (auto&& child : children) {
        str << '"' << name << "\":";
        str << '[' << value << ',';
        str << ']';
            
    }
}

std::string call_trie::get_json() {
    print_children(m_trie.get_children());
}
