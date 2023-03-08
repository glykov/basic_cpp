#include <exception>
#include <cassert>
#include <cstring>
#include "proxy.h"

const std::regex ProxyServer::uri_regexp(R"_((?:([^\:]*)\://)?(?:([^\:\@]*)(?:\:([^\@]*))?\@)?(?:([^/\:]*)\.(?=[^\./\:]*\.[^\./\:]*))?([^\./\:]*)(?:\.([^/\.\:]*))?(?:\:([0-9]*))?(/[^\?#]*(?=.*?/)/)?([^\?#]*)?(?:\?([^#]*))?(?:#(.*))?)_",
                                         std::regex_constants::ECMAScript | std::regex_constants::icase);


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

void ProxyServer::client_error(Socket &sock, const std::string &cause, int err_num, const std::string &short_message,
                               const std::string &long_message) const
{
    std::string err_headers = "HTTP/1.0 " + std::to_string(err_num) + " " + short_message + "\r\n";
    std::stringstream err_body_s;
    err_body_s
            << "<html><title>Proxy Error</title>"
            << "<body bgcolor=0xffffff>\r\n"
            << err_num <<  ": " << short_message << "\r\n"
            << "<p>" << long_message << ": " << cause << "\r\n"
            << "<hr><em>The GB Student Proxy Server</em>\r\n"
            << "</body></html>\r\n";

    // Print the HTTP response.
    send(sock, &err_headers.at(0), err_headers.size(), 0);

    err_headers = "Content-type: text/html\r\n";
    send(sock, &err_headers.at(0), err_headers.size(), 0);

    auto err_body = err_body_s.str();

    err_headers = "Content-length: " + std::to_string(err_body.size()) + "\r\n\r\n";
    send(sock, &err_headers.at(0), err_headers.size(), 0);
    send(sock, &err_body.at(0), err_body.size(), 0);
}


ProxyServer::uri_data ProxyServer::parse_uri(const std::string &uri) const
{
    std::smatch uri_match;
    if (!std::regex_match(uri, uri_match, uri_regexp)) return std::make_tuple("", "./", 0);

    std::string host_name = uri_match[5];
    std::stringstream host_name_s;

    if (uri_match[4].matched) host_name_s << uri_match[4] << ".";
    host_name_s << uri_match[5];
    if (uri_match[6].matched) host_name_s << "." << uri_match[6];

    std::string path = uri_match[9].str();
    host_name = host_name_s.str();

    return std::make_tuple(host_name,
                           path.size() ? path : "/",
                           uri_match[7].matched ? std::stoi(uri_match[7]) : default_target_port);
}


std::tuple<std::string, std::string> ProxyServer::parse_request_headers(Socket &s) const
{
    std::string line;
    std::stringstream result;
    std::string host_name;

    static const std::string host_header_name = "Host: ";

    do
    {
        line = read_line(s);

        if (("\r\n" == line) || ("\n" == line))
            continue;

        if (line.find("User-Agent:") != std::string::npos)
            continue;

        if (line.find("Accept:") != std::string::npos)
            continue;

        if (line.find("Accept-Encoding:") != std::string::npos)
            continue;

        if (line.find("Connection:") != std::string::npos)
            continue;

        if (line.find("Proxy-Connection:") != std::string::npos)
            continue;

        auto host_pos = line.find(host_header_name);
        if (host_pos != std::string::npos)
        {
            host_name = line.substr(host_pos + host_header_name.length());
            continue;
        }

        result << line;
    } while ((line != "\r\n") && (line != "\n"));

    return make_tuple(result.str(), host_name);
}


void ProxyServer::proxify(Socket& client_socket)
{
    std::string method;
    std::string uri;
    std::string version;

    std::cout << "Waiting for client request..." << std::endl;

    try
    {
        auto line = read_line(client_socket);

        std::stringstream ss(line);
        ss >> method >> uri >> version;

        std::cout
                << "Client request: \"" << line << "\" parsed.\n"
                << "Method = " << method << "\n"
                << "URI = " << uri << "\n"
                << "Version = " << version
                << std::endl;

        // This proxy currently only supports the GET call.
        if (method != "GET")
        {
            std::cerr << "Unknown method: \"" << method << "\"" << std::endl;
            client_error(client_socket, method, 501, "Not implemented", "This proxy does not implement this method");
            return;
        }

        // Parse the HTTP request to extract the hostname, path and port.
        auto [host_name, path, port] = parse_uri(uri);

        std::cout
                << "Host name from the request = " << host_name << "\n"
                << "Path = " << path << "\n"
                << "Port from the request = " << port
                << std::endl;

        auto [new_headers, host_name_from_header] = parse_request_headers(client_socket);

        if (host_name_from_header.size())
        {
            std::tie(host_name, std::ignore, port) = parse_uri(host_name_from_header);
            std::cout
                    << "Host from the header = " << host_name << "\n"
                    << "Port from the header = " << port
                    << std::endl;
        }

        // write client request to file
        std::ofstream fout("log.txt", std::ios::app);
        fout << line;
        // ======================================================

        // Create new connection with server.
        auto&& proxy_to_server_socket = connect_to_target_server(host_name, port);

        std::cout
                << "Connected.\n\n"
                << "Writing HTTP request to the target server..."
                << std::endl;

        if (send(proxy_to_server_socket, &line.at(0), line.size(), 0) < 0)
        {
            auto s = "client error";
            std::cerr << s << std::endl;
            client_error(client_socket, method, 503, "Internal error", s);
            return;
        }

        std::cout
                << "Request was written.\n\n"
                << "Reading response from the target server..."
                << std::endl;

        std::string response;

        do
        {
            // HTTP 1.x is a text protocol.
            line = read_line(proxy_to_server_socket);
            response += line;
        } while (line.size());

        // write server response to file
        fout << response;
        //=================================================

        std::cout
                << "Response was read.\n"
                << "=======================\n"
                << "Target server response:\n"
                << "-----------------------\n"
                << response
                << "\n=======================\n"
                << std::endl;

        std::cout
                << "Forwarding response to the client..."
                << std::endl;

        send(client_socket, &response.at(0), response.size(), 0);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown exception in the client thread!" << std::endl;
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