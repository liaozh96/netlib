#ifndef __NETLIB_NONCOPYABLE_HPP__
#define __NETLIB_NONCOPYABLE_HPP__

namespace netlib
{

class Noncopyable
{
protected:
    Noncopyable(){}
    ~Noncopyable(){}

    Noncopyable(const Noncopyable &) = delete ;
    Noncopyable & operator=(const Noncopyable &) = delete;
};

class Copyable
{};

}//end of namespace netlib

#endif
