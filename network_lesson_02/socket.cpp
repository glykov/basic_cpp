#include <utility>
#include "socket.h"

Socket::Socket(int domain, int type, int protocol) : sockfd(INVALID_SOCKET)
{
    open(domain, type, protocol);
}

Socket::Socket(int socket_descriptor) : sockfd(socket_descriptor)
{
}

Socket::Socket(Socket &&socket)
{
    sockfd = socket.sockfd;
    socket.sockfd = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket &&socket)
{
    if (&socket == this) return *this;

    if (opened()) close();
    std::swap(sockfd, socket.sockfd);

    return *this;
}

Socket::~Socket()
{
    if (opened()) close();
}

bool Socket::opened() const
{
    return sockfd != INVALID_SOCKET;
}

void Socket::open(int domain, int type, int protocol)
{
    if (opened()) close();

    sockfd = socket(domain, type, protocol);
}

int Socket::close()
{
    int status = 0;

    status = shutdown(sockfd, SHUT_RDWR);
    sockfd = INVALID_SOCKET;

    return status;
}