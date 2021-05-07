#include "Thread.hpp"
#include <stdio.h>
#include <string>

namespace netlib
{

Thread::Thread(ThreadFunc func, const std::string &n)
: _isRunning(false)
, _pthid(0){}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);
    }
}

void Thread::

}//end of namespace netlib
