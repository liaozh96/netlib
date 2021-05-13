#ifndef __NETLIB_INETADDRESS_HPP__
#define __NETLIB_INETADDRESS_HPP__

#include <netinet/in.h>

#include <string>

using std::string;

namespace netlib
{

class InetAddress
{
public:
    InetAddress(const string &ip, unsigned short port);
    InetAddress(const struct sockaddr_in & addr);

    string ip() const;
    unsigned short port() const;

    const struct sockaddr_in *getInetAddressPtr() const;

private:
    struct sockaddr_in _addr;
};

}//end of namespace netlib

#endif
