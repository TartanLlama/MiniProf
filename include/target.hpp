// Copyright (C) 2002-2016 Codeplay Software Limited. All Rights Reserved.

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace miniprof {
    using addr_t = uint64_t;
    
    class function {
    public:
        function (std::string name, addr_t address) :
            m_name{std::move(name)}, m_address{address} {}
        
        const std::string& get_name() { return m_name; };
        addr_t get_start_address() { return m_address; };

    private:
        std::string m_name;
        addr_t m_address;
    };

    class line_entry {
    public:
        line_entry (std::string file_name, uint64_t line) :
            m_file_name{std::move(file_name)}, m_line{line} {}
        
        const std::string& get_file_name() { return m_file_name; };
        addr_t get_line() { return m_line; };

    private:
        std::string m_file_name;
        uint64_t m_line;
    };
    
    class stack_frame {
    public:
        virtual ~stack_frame() = default;

        virtual function get_function() = 0;
        virtual line_entry get_line_entry() = 0;
    };
    
    class thread {
    public:
        virtual ~thread() = default;

        virtual std::vector<std::unique_ptr<stack_frame>> get_frames() = 0;
    };
    
    class process {
    public:
        virtual ~process() = default;

        using thread_collection = std::vector<std::unique_ptr<thread>>;
        
        virtual thread_collection get_threads() = 0;
        virtual void start() = 0;
        virtual void halt() = 0;
        virtual bool is_finished() = 0;
    };

    class process_launcher {
    public:
        virtual ~process_launcher() = default;
    
        virtual std::unique_ptr<process> attach (uint32_t pid) = 0;
        virtual std::unique_ptr<process> launch (const std::string& file_name, const std::string& args) = 0;

        static std::unique_ptr<process_launcher>
        get_process_launcher_plugin (const std::string& name) {
            return s_plugin_map[name]();
        }

        static uint32_t
        register_process_launcher_plugin (const std::string& name,
                                          std::function<std::unique_ptr<process_launcher>()> creation_function) {
            s_plugin_map[name] = creation_function;
            return s_current_plugin_id++;
        }

        static std::vector<std::string> get_plugin_names() {
            std::vector<std::string> names;
            for (auto&& e : s_plugin_map) {
                names.push_back(e.first);
            }

            return names;
        }

        static uint32_t s_current_plugin_id;
        static std::unordered_map<std::string, std::function<std::unique_ptr<process_launcher>()>> s_plugin_map;
    };
}
