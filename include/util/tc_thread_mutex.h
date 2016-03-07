#ifndef __TC_THREAD_MUTEX_H
#define __TC_THREAD_MUTEX_H

#include "util/tc_lock.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 线程锁互斥类(修改至ICE源码)
// Author : j@syswin.com              
/////////////////////////////////////////////////
class TC_ThreadCond;

/**
 * 线程互斥对象
 */
struct TC_ThreadMutex_Exception : public TC_Lock_Exception
{
    TC_ThreadMutex_Exception(const string &buffer) : TC_Lock_Exception(buffer){};
    TC_ThreadMutex_Exception(const string &buffer, int err) : TC_Lock_Exception(buffer, err){};
    ~TC_ThreadMutex_Exception() throw() {};
};

/**
* 线程锁
*/
class TC_ThreadMutex
{
public:

    TC_ThreadMutex();
    virtual ~TC_ThreadMutex();

    /**
     * 加锁
     */
    void lock() const;

    /**
     * 尝试锁
     * 
     * @return bool
     */
    bool tryLock() const;

    /**
     * 解锁
     */
    void unlock() const;

    /**
     * 加锁后调用unlock是否会解锁, 给TC_Monitor使用的
     * 永远返回true
     * 
     * @return bool
     */
    bool willUnlock() const { return true;}

protected:

    // noncopyable
    TC_ThreadMutex(const TC_ThreadMutex&);
    void operator=(const TC_ThreadMutex&);

	/**
     * 计数
	 */
    int count() const;

    /**
     * 计数
	 */
    void count(int c) const;

    friend class TC_ThreadCond;

protected:
    mutable pthread_mutex_t _mutex;

};

/**
* 线程锁类
* 采用线程库实现
**/
class TC_ThreadRecMutex
{
public:

    /**
    * 构造函数
    */
    TC_ThreadRecMutex();

    /**
    * 析够函数
    */
    virtual ~TC_ThreadRecMutex();

    /**
    * 锁, 调用pthread_mutex_lock
    * return : 返回pthread_mutex_lock的返回值
    */
    int lock() const;

    /**
    * 解锁, pthread_mutex_unlock
    * return : 返回pthread_mutex_lock的返回值
    */
    int unlock() const;

    /**
    * 尝试锁, 失败抛出异常
    * return : true, 成功锁; false 其他线程已经锁了
    */
    bool tryLock() const;

    /**
     * 加锁后调用unlock是否会解锁, 给TC_Monitor使用的
     * 
     * @return bool
     */
    bool willUnlock() const;
protected:

	/**
     * 友元类
     */
    friend class TC_ThreadCond;

	/**
     * 计数
	 */
    int count() const;

    /**
     * 计数
	 */
    void count(int c) const;

private:
    /**
    锁对象
    */
    mutable pthread_mutex_t _mutex;
	mutable int _count;
};

}

#endif

