#include "SocketIO.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

using std::cout;
using std::endl;

namespace netlib
{

int SocketIO::readn(char *buff, int len)
{
    int left = len;//剩下left个字节数没有获取到
    char *p = buff;

    int ret = -1;
    while(left > 0)
    {
        ret = read(_fd, p, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("read");
            return len - left;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            p += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SocketIO::writen(const char *buff, int len)
{
    int left = len;
    const char *p = buff;

    int ret = -1;
    while(left > 0)
    {
        ret = write(_fd, p, left);
        if(-1 == ret && EINTR == errno)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("sned");
            return len - left;
        }
        else 
        {
            p += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SocketIO::readline(char *buff, int maxlen)
{
    int left = maxlen - 1;//留一个字节存放'\0'
    char *p = buff;
    int ret = -1;
    int total = 0;

    while(left > 0)
    {
        //MSG_PEEK预取内核接收缓冲区中的数据，而不移走数据
        ret = recv(_fd, p, left, MSG_PEEK);
        if(-1 == ret && EINTR == errno)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("recv");
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            for(int idx = 0; idx < ret; ++idx)
            {
                if(p[idx] == '\n')
                {
                    int sz = idx + 1;
                    readn(p,sz);
                    p += sz;
                    *p = '\0';
                    return total +sz;
                }
            }
            readn(p,ret);
            left -= ret;
            total +=ret;
            p += ret;
        }
    }
    *p = '\0';
    return total - left;
}

}//end of namespace netlib
