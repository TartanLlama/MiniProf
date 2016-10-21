// Copyright (C) 2002-2016 Codeplay Software Limited. All Rights Reserved.


#include <iostream>
#include "target.hpp"
#include "lldb_target.hpp"

static constexpr uint32_t event_wait_timeout = 3;

using namespace miniprof;

const uint32_t lldb_process_launcher::s_plugin_id = process_launcher::register_process_launcher_plugin("lldb",
                                        [](){return std::unique_ptr<process_launcher>(new lldb_process_launcher);});

lldb_process_launcher::lldb_process_launcher() {
    lldb::SBDebugger::Initialize();
    m_debugger = lldb::SBDebugger::Create();
}

std::unique_ptr<process>
lldb_process_launcher::attach (uint32_t pid) {
    auto target = m_debugger.FindTargetWithProcessID (pid);
    lldb::SBError err{};
    lldb::SBAttachInfo info{};
    return std::make_unique<lldb_process>(target.Attach(info, err));
}

std::unique_ptr<process>
lldb_process_launcher::launch (const std::string& file_name, const std::string& args) {
    lldb::SBError err{};
    auto target = m_debugger.CreateTarget(file_name.c_str(), nullptr, nullptr, true, err);
    auto data = args.data();
    lldb::SBLaunchInfo info{nullptr};
    info.SetLaunchFlags(lldb::eLaunchFlagStopAtEntry);
    
    auto process = std::make_unique<lldb_process>(target.Launch(info, err));

    return process;
}

std::vector<std::unique_ptr<thread>>
lldb_process::get_threads() {
    std::vector<std::unique_ptr<thread>> threads;
    threads.reserve(m_process.GetNumThreads());

    for (size_t i = 0; i < m_process.GetNumThreads(); ++i) {
        threads.push_back(std::make_unique<lldb_thread>(m_process.GetThreadAtIndex(i)));
    }

    return threads;
}

void lldb_process::start() {
    m_process.Continue();
    
    auto listener = m_process.GetTarget().GetDebugger().GetListener();
    lldb::SBEvent event;
    listener.WaitForEvent(event_wait_timeout, event);
/*    lldb::SBStream stream;
    stream.RedirectToFileHandle(stderr, false);
    event.GetDescription(stream);*/
}

void lldb_process::halt() {
    m_process.Stop();

    auto listener = m_process.GetTarget().GetDebugger().GetListener();
    lldb::SBEvent event;
    listener.WaitForEvent(event_wait_timeout, event);
/*    lldb::SBStream stream;
    stream.RedirectToFileHandle(stderr, false);
    event.GetDescription(stream);*/
}

bool lldb_process::is_finished() {
    auto s = m_process.GetState();
    return s == lldb::eStateCrashed || s == lldb::eStateExited || s == lldb::eStateDetached;
}

std::vector<std::unique_ptr<stack_frame>>
lldb_thread::get_frames() {
    std::vector<std::unique_ptr<stack_frame>> frames;
    frames.reserve(m_thread.GetNumFrames());

    for (size_t i = 0; i < m_thread.GetNumFrames(); ++i) {
        frames.push_back(std::make_unique<lldb_stack_frame>(m_thread.GetFrameAtIndex(i)));
    }

    return frames;
}

function lldb_stack_frame::get_function() {
    auto func = m_frame.GetFunction();
    if (!func.IsValid()) {
        return { "None", 0 };
    }
    
    return { func.GetName(), func.GetStartAddress().GetLoadAddress(m_frame.GetThread().GetProcess().GetTarget()) };
}

line_entry lldb_stack_frame::get_line_entry() {
    auto entry = m_frame.GetLineEntry();
    return { entry.GetFileSpec().GetFilename(), entry.GetLine() };
}

