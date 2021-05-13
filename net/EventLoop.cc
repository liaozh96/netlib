#include "EventLoop.hpp"
#include "TcpConnection.hpp"
#include "Acceptor.hpp"

#include <unistd.h>

namespace netlib
{

EventLoop::EventLoop(Acceptor &acceptor)
: _efd(createEpollfd())
, _acceptor(acceptor)
, _isLooping(false)
, _evtList(1024)
{
    addEpollReadFd(_acceptor.fd());
}

EventLoop::~EventLoop()
{
    if(_efd)
    {
        close(_efd);
    }
}

void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollfd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::setConnectionCallback(TcpConnectionCallback &&cb)
{
    _onConnectionCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}

void EventLoop::waitEpollfd()
{
    int nready = 0;
    do {
        nready = ::epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), 5000);
    }while(-1 == nready && EINTR == errno);
    
    if(-1 == nready)
    {
        perror("epoll_wait");
        return;
    }
    else if(0 == nready){
        printf(">> epoll_wait timeout!\n");
    }else{
        if(nready == _evtList.size())
        {
            _evtList.resize(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd() &&
               _evtList[idx].events &EPOLLIN)
            {
                handleNewConnection();
            }else{
                if(_evtList[idx].events &EPOLLIN)
                {
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);
    TcpConnectionPtr conn(new TcpConnection(peerfd));

    conn->setConnectionCallback(_onConnectionCb);
    conn->setMessageCallback(_onMessageCb);
    conn->setCloseCallback(_onCloseCb);

    _conns.insert(std::make_pair(peerfd, conn));
    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    //1.通过fd找到TcpConnection对象
    auto iter = _conns.find(fd);
    if(iter != _conns.end())
    {
        //2.判断该链接是否断开
        bool isClosed = iter->second->isClosed();
        //2.1 如果断开，执行连接断开时的事件处理器
        if(isClosed)
        {
            iter->second->handleCloseCallback();
            delEpollReadFd(fd);
            _conns.erase(iter);
        }else {
        //2.2 如果连接没有断开，执行消息到达时的事件处理器
            iter->second->handleMessahgeCallback();
        }
    }
}

}//end if namespace netlib
