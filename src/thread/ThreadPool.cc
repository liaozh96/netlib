#include "ThreadPool.hpp"

#include <unistd.h>

namespace netlib
{

Threadpool::Threadpool(size_t threadNum, size_t queueSize)
: _threadNum(threadNum)
, _queueSize(queueSize)
, _taskqueue(_queueSize)
, _isExit(false)
{
    _threads.reserve(_threadNum);
}

Threadpool::~Threadpool()
{
    if(!_isExit)
    {
        stop();
    }
}

void Threadpool::start()
{
    for(size_t idx = 0; idx != _threadNum; ++idx)
    {
        unique_ptr<Thread> up(new Thread(std::bind(&Threadpool::threadFunc, this)));
        _threads.push_back(std::move(up));
    }

    for(auto &pthread : _threads)
    {
        pthread->start();
    }
}

void Threadpool::addTask(Task &&task)
{
    if(task)
    {
        _taskqueue.push(std::move(task));
    }
}

Task Threadpool::getTask()
{
    return _taskqueue.pop();
}

void Threadpool::stop()
{
    while(!_taskqueue.empty())
    {
        sleep(1);
    }

    _isExit = true;
    _taskqueue.wakeup();
    for(auto &pthread : _threads)
    {
        pthread->join();
    }
}

void Threadpool::threadFunc()
{
    while(!_isExit)
    {
        Task taskcb = getTask();
        if(taskcb)
        {
            taskcb();
        }
    }
}

}//end of namespace netlib
