// Copyright (C) 2002-2016 Codeplay Software Limited. All Rights Reserved.

#pragma once

#include <lldb/API/LLDB.h>
#include "target.hpp"

namespace miniprof {
    class lldb_stack_frame : public stack_frame {
    public:
        explicit lldb_stack_frame (const lldb::SBFrame& frame)
            : m_frame{frame} {}
        
        function get_function() override;
        line_entry get_line_entry() override;

    private:
        lldb::SBFrame m_frame;
    };
    
    class lldb_thread : public thread {
    public:
        explicit lldb_thread (const lldb::SBThread& thread)
            : m_thread{thread} {}
        
        std::vector<std::unique_ptr<stack_frame>> get_frames() override;

    private:
        lldb::SBThread m_thread;
    };
    
    class lldb_process : public process {
    public:
        explicit lldb_process (const lldb::SBProcess& process)
            : m_process{process} {}
        
        std::vector<std::unique_ptr<thread>> get_threads() override;
        void start() override;
        void halt() override;
        bool is_finished() override;
        
    private:
        lldb::SBProcess m_process;
    };

    class lldb_process_launcher : public process_launcher{
    public:
        lldb_process_launcher();
        
        std::unique_ptr<process> attach (uint32_t pid) override;
        std::unique_ptr<process> launch (const std::string& file_name, const std::string& args) override;

    private:
        lldb::SBDebugger m_debugger;
        
        static const uint32_t s_plugin_id;
    };
}
