#include "Acceptor.hpp"

#include <unistd.h>

namespace netlib
{

Acceptor::Acceptor(const string &ip, unsigned short port)
: _listensock()
, _serverAddr(ip,port){}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

int Acceptor::accept()
{
    int peerfd = ::accept(fd(), nullptr, nullptr);
    if(peerfd < 0)
    {
        perror("accept");
    }
    return peerfd;
}

void Acceptor::setReuseAddr()
{
    int on = 1;
    if(setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("setsockopt reuseaddr");
    }
}

void Acceptor::setReusePort()
{
    int on = 1;
    if(setsockopt(fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) < 0)
    {
        perror("setsockopt reuseport");
    }
}

void Acceptor::bind()
{
    if(::bind(fd()
        ,(struct sockaddr*)_serverAddr.getInetAddressPtr()
         ,sizeof(InetAddress)) < 0)
    {
        perror("bind");
    }
}

void Acceptor::listen()
{
    if(::listen(fd(),128) < 0)
    {
        perror("listen");
    }
}

}//end of namespace netlib

