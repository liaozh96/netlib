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
    
    using ThreadCallback = std::function<void ()>;
    
    //右值引用是为了减少赋值的开销
    Thread(ThreadCallback && thFunc);
    ~Thread();

    void start();
    void join();

private:

    //不希望该方法在类之外被调用
    //同时消除this指针的影响
    static void *ThreadFunc(void*);

private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cbFunc;
};

}//end of namespace netlib

#endif
