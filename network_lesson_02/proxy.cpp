#include <exception>
#include <cassert>
#include "proxy.h"

ProxyServer::ProxyServer(unsigned short port) : port_(port), sock_(AF_INET, SOCK_STREAM, 0)
{
    if (!sock_) {
        throw std::logic_error("failed to create proxy");
    }
}

ProxyServer::~ProxyServer()
{
    stop();
}

void ProxyServer::start()
{
    if (!sock_) {
        throw std::logic_error("invalid socket");
    }

    started_ = true;

    sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port_)
    };
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0) {
        throw std::logic_error("cannot bind to socket");
    }

    if (listen(sock_, back_log) < 0) {
        throw std::logic_error("canot listen to socket");
    }

    std::cout << "Listening to the port " << port_ << "..." << std::endl;

    while (started_) {
        sockaddr_in client_addr = { 0 };
        socklen_t client_len = sizeof(client_addr);

        Socket client_sock(accept(sock_, reinterpret_cast<sockaddr*>(&client_addr), &client_len));

        std::cout << "Accepted connection..." << std::endl;

        if (!client_sock) {
            throw std::logic_error("client socket failed");
        }

        proxify(client_sock);

//        std::thread client_thread(&ProxyServer::proxify, this, std::move(client_sock));
//        std::cout << "Creating client thread..." << std::endl;
//        client_thread.join();
    }

    sock_.close();
}

void ProxyServer::stop()
{
    started_ = false;
}

std::string ProxyServer::read_line(Socket& socket) const
{
    ssize_t read_bytes;
    std::string result;
    char ch;

    while (true) {
        read_bytes = ::recv(socket, &ch, 1, 0);
        if (read_bytes == -1) {
            if (errno == EINTR) continue;
            else throw std::logic_error("error reading bytes");
        } else if (read_bytes == 0) {
            break;
        } else {
            result += ch;
            if (ch == '\n') break;
        }
    }

    return result;
}

void ProxyServer::proxify(Socket& client_socket)
{
    std::string method;
    std::string uri;
    std::string version;

    std::cout << "Waiting for client to connect..." << std::endl;

    try {
        std::string line = read_line(client_socket);
    }
}

bool ProxyServer::try_to_connect(Socket& socket, const sockaddr* addr, size_t addr_len)
{
    if (connect(socket, addr, addr_len) == -1) {
        std::cerr << "Connection failed in try_to_connect" << std::endl;
        return false;
    }
    return true;
}

Socket ProxyServer::connect_to_target_server(const std::string &host_name, unsigned short port)
{
    addrinfo hints = {
        .ai_flags = AI_CANONNAME,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    addrinfo *s_i = nullptr;
    int status = 0;

    if ((status = getaddrinfo(host_name.c_str(), std::to_string(port).c_str(), &hints, &s_i)) != 0) {
        std::string msg{"getaddrinfo error: "};
        msg += gai_strerror(status);
        throw std::logic_error(msg);
    }

    std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> servinfo {s_i, freeaddrinfo};

    for (auto const *s = servinfo.get(); s != nullptr; s = s->ai_next) {
        assert(s->ai_family == s->ai_addr->sa_family);

        if (s->ai_family == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            sockaddr_in *const sin = reinterpret_cast<sockaddr_in *const>(s->ai_addr);
            in_addr addr;
            addr.s_addr = *reinterpret_cast<const in_addr_t *>(&sin->sin_addr);
            sin->sin_family = AF_INET;
            sin->sin_port = htons(port);

            std::cout << "Trying IP Address: " << inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN) << std::endl;
            Socket sock = {AF_INET, SOCK_STREAM, IPPROTO_TCP};
            if (try_to_connect(sock, reinterpret_cast<const sockaddr *>(&sin), sizeof(sockaddr_in))) {
                return sock;
            }
        } else if (s->ai_family == AF_INET6) {
            char ip6[INET6_ADDRSTRLEN];
            sockaddr_in6 *const sin = reinterpret_cast<sockaddr_in6 *const>(s->ai_addr);
            sin->sin6_family = AF_INET6;
            sin->sin6_port = htons(port);

            std::cout << "Trying IPv6 Address: " << inet_ntop(AF_INET6, &(sin->sin6_addr), ip6, INET_ADDRSTRLEN)
                      << std::endl;
            Socket sock = {AF_INET6, SOCK_STREAM, IPPROTO_TCP};
            if (try_to_connect(sock, reinterpret_cast<const sockaddr *>(&sin), sizeof(sockaddr_in6))) {
                return sock;
            }
        }
    }
    throw std::logic_error("connection error");
}