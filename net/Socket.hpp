#ifndef __NETLIB_SOCKET_HPP__
#define __NETLIB_SOCKET_HPP__

#include <boost/noncopyable.hpp>

namespace netlib
{

class Socket
: boost::noncopyable
{
public:
    Socket();
    Socket(int fd);
    ~Socket();

    int fd() const 
    {
        return _fd;
    }

    void shutdownWrite();

private:
    int _fd;

};

}//end of namespace netlib

#endif
