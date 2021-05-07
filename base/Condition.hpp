#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include "MutexLock.hpp"
#include <boost/noncopyable.hpp>

namespace netlib
{


class Condition
: boost::noncopyable
{
public:
    Condition(MutexLock &mutex)
    : _mutex(mutex)
    {
        pthread_cond_init(&_pcond,NULL);
    }

    ~Condition()
    {
        pthread_cond_destroy(&_pcond);
    }

    void wait()
    {
        pthread_cond_wait(&_pcond,_mutex.getThreadMutex());
    }

    void notify()
    {
        pthread_cond_signal(&_pcond); 
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&_pcond);
    }

private:
    MutexLock &_mutex;
    pthread_cond_t _pcond;
};




}// end of namespace netlib




#endif
