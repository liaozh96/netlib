#include "Thread.hpp"

#include <stdio.h>
#include <string>

namespace netlib
{

Thread::Thread(ThreadCallback &&cbFunc)
: _pthid(0)
, _isRunning(false)
, _cbFunc(std::move(cbFunc)){}//注册回调函数

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);
    }
}

//star函数有this指针
void Thread::start()
{
    if(pthread_create(&_pthid, nullptr, ThreadFunc, this))
    {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void *Thread::ThreadFunc(void *arg)
{
    Thread *pthread = static_cast<Thread*>(arg);
    if(pthread)
    {
        pthread->_cbFunc();
    }
    return nullptr;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid,nullptr);
        _isRunning = false;
    }
}

}//end of namespace netlib
