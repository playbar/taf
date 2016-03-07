#ifndef _TC_THREAD_COND_H
#define _TC_THREAD_COND_H

#include <sys/time.h>
#include <cerrno>
#include <iostream>
#include "util/tc_ex.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 条件变量类(修改至ICE源码)
// Author : j@syswin.com  
// Modify : j@syswin.com                
/////////////////////////////////////////////////
class TC_ThreadMutex;

/**
 * 线程条件异常类
 */
struct TC_ThreadCond_Exception : public TC_Exception
{
    TC_ThreadCond_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_ThreadCond_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_ThreadCond_Exception() throw() {};
};

/**
 * 线程条件类, 所有锁可以在上面等待信号发生
 */
class TC_ThreadCond
{
public:

    /**
     * 构造函数
     */
    TC_ThreadCond();

    /**
     * 析构函数
     */
    ~TC_ThreadCond();

    /**
     * 发送信号, 等待在该条件上的一个线程会醒
     */
    void signal();

    /**
     * 等待在该条件的所有线程都会醒
     */
    void broadcast();

    /**
     * 获取绝对等待时间
     */
    timespec abstime(int millsecond) const;

    /**
     * 无限制等待
     * @param M
     */
    template<typename Mutex>
    void wait(const Mutex& mutex) const
    {
        int c = mutex.count();
        int rc = pthread_cond_wait(&_cond, &mutex._mutex);
        mutex.count(c);
        if(rc != 0)
        {
            throw TC_ThreadCond_Exception("[TC_ThreadCond::wait] pthread_cond_wait error", errno);
        }
    }

    /**
     * 等待时间
     * @param M
     *
     * @return bool, false表示超时, true:表示有事件来了
     */
    template<typename Mutex>
    bool timedWait(const Mutex& mutex, int millsecond) const
    {
        int c = mutex.count();

        timespec ts = abstime(millsecond);

        int rc = pthread_cond_timedwait(&_cond, &mutex._mutex, &ts);

        mutex.count(c);

        if(rc != 0)
        {
            if(rc != ETIMEDOUT)
            {
                throw TC_ThreadCond_Exception("[TC_ThreadCond::timedWait] pthread_cond_timedwait error", errno);
            }

            return false;
        }
        return true;
    }

protected:
    // Not implemented; prevents accidental use.
    TC_ThreadCond(const TC_ThreadCond&);
    TC_ThreadCond& operator=(const TC_ThreadCond&);

private:

    /**
     * 线程条件
     */
    mutable pthread_cond_t _cond;

};

}

#endif

