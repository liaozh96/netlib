#ifndef __MUTEXLCOK_HPP_
#define __MUTEXLOCK_HPP_

#include <pthread.h>
#include <boost/utility.hpp>

namespace netlib
{

class MutexLock
: boost::noncopyable
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

}//end of namespace netlib
#endif
