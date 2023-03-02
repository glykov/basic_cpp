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
    const size_t back_log = 10;

    ProxyServer(unsigned short port = 8080);
    ~ProxyServer();

    void start();
    void stop();

    std::string read_line(Socket& socket) const;
    void proxify(Socket& client_socket);
private:
    bool try_to_connect(Socket& sock, const sockaddr* addr, size_t addr_size);
    Socket connect_to_target_server(const std::string& host_name, unsigned short port);

    unsigned short port_;
    bool started_ = false;
    Socket sock_;
};