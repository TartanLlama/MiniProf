#pragma once

#include <boost/network/protocol/http/server.hpp>
#include <boost/range/istream_range.hpp>
#include <iostream>
#include <fstream>

static const std::string g_html_folder = "../html/";
static const std::string g_server_address = "127.0.0.1";
static const std::string g_server_port = "8000";

namespace http = boost::network::http;

namespace miniprof {
    struct data_request_handler;
}
typedef http::server<miniprof::data_request_handler> server;

namespace miniprof {
    class data_server {
    public:
        ~data_server() {
            if (m_thread.joinable())
                m_thread.join();
        }
        void run();
    private:
        std::thread m_thread;
    };
     
    struct data_request_handler {
        void operator()(server::request const &request, server::connection_ptr connection);
    };
}
