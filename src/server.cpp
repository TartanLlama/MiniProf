#include "server.hpp"
#include "data_provider_registry.hpp"

using namespace miniprof;

void data_server::run() {
    m_thread = std::thread([](){
            data_request_handler handler{};
            server::options options(handler);
            server server_(options.address(g_server_address).port(g_server_port));
            server_.run();
        });
}


void data_request_handler::operator()(server::request const &request, server::connection_ptr connection) {
    if (request.destination == "/") {
        std::ifstream file {g_html_folder + "index.html"};
        std::stringstream buff;
        buff << file.rdbuf();
        connection->write(buff.str());
    }
    else if (request.destination == "/jquery-3.1.1.min.js") {
        std::ifstream file {g_html_folder + "jquery-3.1.1.min.js"};
        std::stringstream buff;
        buff << file.rdbuf();
        connection->write(buff.str());
    } else {
        auto data_provider = data_provider_registry::get_data_provider(std::string{request.destination,1});
        if (data_provider) {
            connection->write(data_provider->get_json());
        }
    }
}
