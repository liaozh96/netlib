#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__


#include "TaskQueue.hpp"
#include "Thread.hpp"
#include "Task.hpp"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

namespace netlib
{

class Threadpool
{
public:
    Threadpool(size_t threadNum, size_t queueSize);
    ~Threadpool();

    void start();
    void stop();
    void addTask(Task &&);

private:
    Task getTask();
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queueSize;

    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskqueue;
    bool _isExit;
};
}//end of namespace netlib

#endif
