#ifndef __NETLIB_TASKQUEUE_HPP__
#define __NETLIB_TASKQUEUE_HPP__

#include "MutexLock.hpp"
#include "Condition.hpp"
#include "Task.hpp"

#include <queue>
#include <functional>

using std::queue;

namespace netlib
{

using ElemType = std::function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t size);
    bool empty() const;
    bool full() const;
    void push(const ElemType &);
    ElemType pop();

    void wakeup();

private:
    size_t _queueSize;
    queue<ElemType> _queue;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;
};

}//end of namespace netlib

#endif
