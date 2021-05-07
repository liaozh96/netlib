#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <functional>
#include <string>

namespace netlib
{

class Thread
: boost::noncopyable
{
public:
    typedef std::function<void ()> ThreadFunc;
    Thread(ThreadFunc, const std::string &name = std::string());
    ~Thread();

    void start();
    int join();
private:
    pthread_t _pthid;
    bool _isRunning;
};



}//end of namespace netlib



#endif
