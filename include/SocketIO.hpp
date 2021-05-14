#ifndef __NETLIB_SOCKETIO_HPP__
#define __NETLIB_SOCKETIO_HPP__

namespace netlib
{

class SocketIO
{
public:
    SocketIO(int fd)
    : _fd(fd){}

    int readn(char *buff, int len);
    int readline(char *buff, int maxlen);
    int writen(const char *buff, int len);

private:
    int _fd;
};

}//end of namespace netlib

#endif
