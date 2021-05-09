#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <functional>


namespace netlib
{

using  Task = std::function<void()>;

}//end of namespace netlib

#endif
