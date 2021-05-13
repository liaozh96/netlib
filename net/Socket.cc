#include "Socket.hpp"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace netlib
{

Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0)
    {
        perror("socket");
    }
}

Socket::Socket(int fd)
: _fd(fd){}

Socket::~Socket()
{
    close(_fd);
}

void Socket::shutdownWrite()
{
    shutdown(_fd, SHUT_WR);
}


}//end of namespace netlib
