#ifndef _TC_MONITOR_H
#define _TC_MONITOR_H

#include "util/tc_thread_mutex.h"
#include "util/tc_thread_cond.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 锁监控器类(修改至ICE源码)
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
 * 监控锁
 */
template <class T, class P>
class TC_Monitor
{
public:

    /**
     * 定义锁控制对象
     */
    typedef TC_LockT<TC_Monitor<T, P> > Lock;
    typedef TC_TryLockT<TC_Monitor<T, P> > TryLock;

    /**
     * 构造函数
     */
    TC_Monitor() : _nnotify(0)
    {
    }

    /**
     * 析够
     */
    virtual ~TC_Monitor()
    {
    }

    /**
     * 锁
     */
    void lock() const
    {
        _mutex.lock();
        _nnotify = 0;
    }

    /**
     * 解锁, 根据上锁的次数通知
     */
    void unlock() const
    {
//        int nnotify = _nnotify;
        notifyImpl(_nnotify);
        _mutex.unlock();
//        notifyImpl(nnotify);
    }

    /**
     * 尝试锁
     *
     * @return bool
     */
    bool tryLock() const
    {
        bool result = _mutex.tryLock();
        if(result)
        {
            _nnotify = 0;
        }
        return result;
    }

    /**
     * 等待
     */
    void wait() const
    {
        notifyImpl(_nnotify);

        try
        {
            _cond.wait(_mutex);
        }
        catch(...)
        {
            _nnotify = 0;
            throw;
        }

        _nnotify = 0;
    }

    /**
     * 等时间
     * @param millsecond
     *
     * @return bool, false:超时了, ture:有事件来了
     */
    bool timedWait(int millsecond) const
    {
        notifyImpl(_nnotify);

        bool rc;

        try
        {
            rc = _cond.timedWait(_mutex, millsecond);
        }
        catch(...)
        {
            _nnotify = 0;
            throw;
        }

        _nnotify = 0;
        return rc;
    }

    /**
     * 通知某一个线程醒来
     * 调用该函数之前必须加锁, 在解锁的时候才真正通知
     */
    void notify()
    {
        if(_nnotify != -1)
        {
            ++_nnotify;
        }
    }

    /**
     * 通知所有的线程醒来
     * 该函数调用前之必须加锁, 在解锁的时候才真正通知
     *
     */
    void notifyAll()
    {
        _nnotify = -1;
    }

protected:

    /**
     * 通知实现
     * @param nnotify
     */
    void notifyImpl(int nnotify) const
    {
        if(nnotify != 0)
        {
            if(nnotify == -1)
            {
                _cond.broadcast();
                return;
            }
            else
            {
                while(nnotify > 0)
                {
                    _cond.signal();
                    --nnotify;
                }
            }
        }
    }

private:

    // noncopyable
    TC_Monitor(const TC_Monitor&);
    void operator=(const TC_Monitor&);

protected:

    mutable int     _nnotify;
    mutable P       _cond;
    T               _mutex;
};

/**
 * 普通线程锁
 */
typedef TC_Monitor<TC_ThreadMutex, TC_ThreadCond> TC_ThreadLock;

/**
 * 循环锁(一个线程可以加多次锁)
 */
typedef TC_Monitor<TC_ThreadRecMutex, TC_ThreadCond> TC_ThreadRecLock;

}
#endif

