#ifndef __NETLIB_EVENTLOOP_HPP__
#define __NETLIB_EVENTLOOP_HPP__

#include <sys/epoll.h>

#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace netlib
{

class Acceptor;
class TcpConnection;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;

class EventLoop
{
    using EventList = std::vector<struct epoll_event>;
    using TcpConnsMap = std::map<int,TcpConnectionPtr>;

public:
    EventLoop(Acceptor &);
    ~EventLoop();

    void loop();
    void unloop();

    void setConnectionCallback(TcpConnectionCallback && cb);
    void setMessageCallback(TcpConnectionCallback && cb);
    void setCloseCallback(TcpConnectionCallback && cb);

private:
    void waitEpollfd();
    void handleNewConnection();
    void handleMessage(int);

    int createEpollfd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

private:
    int _efd;
    Acceptor & _acceptor;
    bool _isLooping;
    EventList _evtList;
    TcpConnsMap _conns;
    
    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

}//end of namespace netlib

#endif
