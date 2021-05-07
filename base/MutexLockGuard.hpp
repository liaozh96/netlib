#ifndef __MUTEXLOCKGUARD_HPP__
#define __MUTEXLOCKGUARD_HPP__

#include "MutexLock.hpp"
#include <boost/noncopyable.hpp>



namespace netlib
{

class MutexLockGuard
: boost::noncopyable
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
