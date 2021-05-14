#ifndef __NETLIB_TCPCONNECTION_HPP__
#define __NETLIB_TCPCONNECTION_HPP__

#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"

#include <string>
#include <memory>
#include <functional>
#include <boost/noncopyable.hpp>

using std::string;

namespace netlib
{

class TcpConnection
: boost::noncopyable
, public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;

public:
    TcpConnection(int fd);
    
    string receive();
    void send(const string & msg);
    bool isClosed() const;
    
    string toString() const;

    void setConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback & cd);
    void setCloseCallback(const TcpConnectionCallback & cb);

    void handleConnectionCallback();
    void handleMessahgeCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onClosCb;
};

}//end of namespace netlib



#endif
