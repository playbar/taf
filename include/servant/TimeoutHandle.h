#ifndef __TAF_TIMEOUT_HANDLE_H_
#define __TAF_TIMEOUT_HANDLE_H_

#include "servant/TMEventHandle.h"
#include <set>

namespace taf
{
/**
 * 超时处理句柄，可注册待响应的超时队列
 */
template<class T>
class TimeoutHandle : public TMEventHandle, public TC_ThreadMutex
{
public:
    /**
     * 超时处理方法
     * @return int
     */
    virtual int handleTimeout() ;

public:
    /**
     * 注册需要触发的队列
     * @param que
     */
    void registerHandle(T* que);

    /**
     * 注销需要触发的队列
     * @param que
     */
    void unregisterHandle(T* que);

protected:
    /**
     * 已注册的队列
     */
    std::set<T*> _queues;
};
////////////////////////////////////////////////////////////////////
template<class T>
int TimeoutHandle<T>::handleTimeout()
{
    typename std::set<T*>::iterator it;

    for (it = _queues.begin(); it != _queues.end(); ++it)
    {
        (*it)->timeout();
    }
    return 0;
}

template<class T>
void TimeoutHandle<T>::registerHandle(T* que)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    _queues.insert(que);
}

template<class T>
void TimeoutHandle<T>::unregisterHandle(T* que)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    typename std::set<T*>::iterator it = _queues.find(que);

    if (it != _queues.end())
    {
        _queues.erase(it);
    }
}
///////////////////////////////////////////////////////
}
#endif
