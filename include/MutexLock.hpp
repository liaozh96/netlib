#ifndef __NETLIB_MUTEXLCOK_HPP_
#define __NETLIB_MUTEXLOCK_HPP_

#include "Noncopyable.hpp"

#include <pthread.h>

namespace netlib
{

class MutexLock
: Noncopyable
{
public:
    MutexLock()
    {
        pthread_mutex_init(&_mutex,NULL);
    }

    ~MutexLock()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }

    //仅限Condition调用
    pthread_mutex_t *getThreadMutex()
    {
        return &_mutex;
    }

private:
    pthread_mutex_t _mutex;
};

//利用RAII技术控制锁的生命周期
class MutexLockGuard
: Noncopyable
{
public:
    MutexLockGuard(MutexLock &mutex)
    : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }

private:
    MutexLock &_mutex;
};


}//end of netlib

#define MutexLockGuard(x) static_assert(false,"missing mutex guard var name")

#endif
