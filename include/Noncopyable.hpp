#ifndef __NONCOPYABLE_HPP__
#define __NONCOPYABLE_HPP__

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

}//end of namespace netlib

#endif
