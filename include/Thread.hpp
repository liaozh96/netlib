#ifndef __NETLIB_THREAD_HPP__
#define __NETLIB_THREAD_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>
#include <functional>
#include <string>

namespace netlib
{

class Thread
: Noncopyable
{
public:
    
    using ThreadCallback = std::function<void()>;
    
    //右值引用是为了减少复制的开销
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
    //线程状态
    //true为线程存在
    //false为线程未创建或已被回收
    bool _isRunning;

    ThreadCallback _cbFunc;
};

}//end of namespace netlib

#endif
