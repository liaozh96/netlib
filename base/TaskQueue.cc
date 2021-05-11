#include "TaskQueue.hpp"



namespace netlib
{

TaskQueue::TaskQueue(size_t size)
: _queueSize(size)
, _queue()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true){}

bool TaskQueue::empty() const
{
    return _queue.size() == 0;
}

bool TaskQueue::full() const
{
    return _queueSize == _queue.size();
}

//push函数运行线程
void TaskQueue::push(const ElemType &e)
{

    //RAII计数解决死锁的问题
    MutexLockGuard autolock(_mutex);
    while(full())//用while防止虚假唤醒
    {
        _notFull.wait();
    }

    _queue.push(e);
    //return
    
    _notEmpty.notify();
}

ElemType TaskQueue::pop()
{
    MutexLockGuard autolock(_mutex);
    while(_flag && empty())
    {
        _notEmpty.wait();
    }

    //先从队列中获取数据，在pop
    if(_flag)
    {
        ElemType tmp = _queue.front();
        _queue.pop();
        //return
        _notFull.notify();
        return tmp;
    }
    else
    {
        return nullptr;
    }
    
}

void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}

}//end of netlib
