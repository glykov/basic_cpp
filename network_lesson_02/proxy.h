#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <tuple>
#include <thread>

#include "socket.h"

class ProxyServer
{
public:
    static const std::regex uri_regexp;
    const unsigned short default_target_port = 80;
    typedef std::tuple<std::string, std::string, unsigned short> uri_data;
    const size_t back_log = 10;

    ProxyServer(unsigned short port = 8080);
    ~ProxyServer();

    void start();
    void stop();

    std::string read_line(Socket& socket) const;
    void proxify(Socket& client_socket);
    void client_error(Socket &sock, const std::string &cause, int err_num, const std::string &short_message,
                      const std::string &long_message) const;

    uri_data parse_uri(const std::string &uri) const;

    std::tuple<std::string, std::string> parse_request_headers(Socket &s) const;
private:
    bool try_to_connect(Socket& sock, const sockaddr* addr, size_t addr_size);
    Socket connect_to_target_server(const std::string& host_name, unsigned short port);

    unsigned short port_;
    bool started_ = false;
    Socket sock_;
};