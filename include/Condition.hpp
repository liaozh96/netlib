#ifndef __NETLIB_CONDITION_HPP__
#define __NETLIB_CONDITION_HPP__

#include "MutexLock.hpp"
#include "Noncopyable.hpp"

namespace netlib
{

class Condition
: Noncopyable
{
public:
    Condition(MutexLock &mutex)
    : _mutex(mutex)
    {
        pthread_cond_init(&_cond, nullptr);
    }

    ~Condition()
    {
        pthread_cond_destroy(&_cond);
    }

    void wait()
    {
        pthread_cond_wait(&_cond, _mutex.getThreadMutex());
    }

    void notify()
    {
        pthread_cond_signal(&_cond); 
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&_cond);
    }

private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};

}// end of namespace netlib

#endif
