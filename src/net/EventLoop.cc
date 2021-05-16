#include "EventLoop.hpp"
#include "TcpConnection.hpp"
#include "Acceptor.hpp"

#include <unistd.h>
#include <sys/eventfd.h>

namespace netlib
{

EventLoop::EventLoop(Acceptor &acceptor)
: _efd(createEpollfd())
, _evfd(createEventfd())
, _acceptor(acceptor)
, _isLooping(false)
, _evtList(1024)
{
    addEpollReadFd(_acceptor.fd());
    addEpollReadFd(_evfd);
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

void EventLoop::runInLoop(Functor &&cb)
{
    //1.打包回调函数，注册给IO线程
    {
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    //2.通知线程IO进行发送
    wakeup();
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
    else if(0 == nready)
    {
        printf(">> epoll_wait timeout!\n");
    }
    else
    {
        if(nready == _evtList.size())
        {
            _evtList.resize(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd() &&
               _evtList[idx].events & EPOLLIN)
            {
                handleNewConnection();
            }
            else if(fd == _evfd)
            {
                handleRead();
                doPendingFunctors();
            }
            else
            {
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
    TcpConnectionPtr conn(new TcpConnection(peerfd, this));

    conn->setConnectionCallback(_onConnectionCb);
    conn->setMessageCallback(_onMessageCb);
    conn->setCloseCallback(_onCloseCb);

    _conns.insert(std::make_pair(peerfd, conn));
    //触发新连接到来时的事件处理器
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
        }
        else
        {
            //2.2 如果连接没有断开，执行消息到达时的事件处理器
            iter->second->handleMessahgeCallback();
        }
    }
}

int EventLoop::createEpollfd()
{
    int fd = epoll_create1(0);
    if(fd < 0)
    {
        perror("epoll_create1");
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &ev);
    if(ret < 0)
    {
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &ev);
    if(ret < 0)
    {
        perror("epoll_ctl");
    }
}

int EventLoop::createEventfd()
{
    int fd = ::eventfd(0,0);
    if(fd < 0)
    {
        perror("create_eventfd");
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = write(_evfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write");
    }
}

void EventLoop::handleRead()
{
    uint64_t howmany = -1;
    int ret = read(_evfd, &howmany, sizeof(howmany));
    if(ret != sizeof(howmany))
    {
        perror("read");
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> tmp;
    {
        MutexLockGuard autolock(_mutex);
        tmp.swap(_pendingFunctors);
    }

    for(auto &cb : tmp)
    {
        cb();
    }
}


}//end if namespace netlib
