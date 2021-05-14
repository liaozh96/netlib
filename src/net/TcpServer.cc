#include "TcpServer.hpp"

#include <iostream>

using std::cout;
using std::endl;

namespace netlib
{

void onConnection(const TcpConnectionPtr & conn)
{
    cout << conn->toString() << " has connected!" << endl;
}

void onMessage(const TcpConnectionPtr & conn)
{
    string msg = conn->receive();
    cout << " >> recv: " << msg;

    Task task(msg);
    threadpool.addTask(std::bind(&Task::process, task));
}

void onClose(const TcpConnectionPtr &conn)
{
    cout << conn->toString() << " has closed!" << endl;
}

}//end of namespace netlib
