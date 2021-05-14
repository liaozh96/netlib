#ifndef __NETLIB_ACCEPTOR_HPP__
#define __NETLIB_ACCEPTOR_HPP__

#include "Socket.hpp"
#include "InetAddress.hpp"


namespace netlib
{

class Acceptor
{
public:
    Acceptor(const string &ip, unsigned short port);

    void ready();
    int accept();
    int fd() const
    {
        return _listensock.fd();
    }

private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();

private:
    Socket _listensock;
    InetAddress _serverAddr;
};

}//end of namespace netlib

#endif
