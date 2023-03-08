#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define INVALID_SOCKET -1

class Socket
{
public:
    Socket(int domain, int type, int protocol);
    Socket(int socket_descriptor);

    Socket(const Socket&) = delete;
    Socket(Socket&& socket);

    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&& socket);

    virtual ~Socket();

    bool opened() const;
    operator bool() const { return opened(); }
    operator int() const { return sockfd; }

    int close();

protected:
    void open(int domain, int type, int protocol);
private:
    int sockfd;
};