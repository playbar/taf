#ifndef _TC_LOCK_H
#define _TC_LOCK_H

#include <string>
#include <stdexcept>
#include <cerrno>
#include "util/tc_ex.h"

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: 锁类
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
* 锁异常
*/
struct TC_Lock_Exception : public TC_Exception
{
    TC_Lock_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_Lock_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_Lock_Exception() throw() {};
};

/**
 * 锁模板类
 */
template <typename T>
class TC_LockT
{
public:

    /**
     * 构造函数
     * @param mutex
     */
    TC_LockT(const T& mutex) : _mutex(mutex)
    {
        _mutex.lock();
        _acquired = true;
    }

    /**
     * 析构
     */
    virtual ~TC_LockT()
    {
        if (_acquired)
        {
            _mutex.unlock();
        }
    }

    /**
     * 上锁, 如果已经上锁,则抛出异常
     */
    void acquire() const
    {
        if (_acquired)
        {
            throw TC_Lock_Exception("thread has locked!");
        }
        _mutex.lock();
        _acquired = true;
    }

    /**
     * 尝试上锁
     *
     * @return bool
     */
    bool tryAcquire() const
    {
        _acquired = _mutex.tryLock();
        return _acquired;
    }

    /**
     * 释放锁, 如果没有上过锁, 则抛出异常
     */
    void release() const
    {
        if (!_acquired)
        {
            throw TC_Lock_Exception("thread hasn't been locked!");
        }
        _mutex.unlock();
        _acquired = false;
    }

    /**
     * 是否已经上锁
     *
     * @return bool
     */
    bool acquired() const
    {
        return _acquired;
    }

protected:

    // TC_TryLockT's contructor
    TC_LockT(const T& mutex, bool) : _mutex(mutex)
    {
        _acquired = _mutex.tryLock();
    }

private:

    // Not implemented; prevents accidental use.
    TC_LockT(const TC_LockT&);
    TC_LockT& operator=(const TC_LockT&);

protected:
    /**
     * 锁对象
     */
    const T&        _mutex;

    /**
     * 是否已经上锁
     */
    mutable bool _acquired;
};

/**
 * 尝试上锁
 */
template <typename T>
class TC_TryLockT : public TC_LockT<T>
{
public:

    TC_TryLockT(const T& mutex) : TC_LockT<T>(mutex, true)
    {
    }
};

/**
 * 空锁, 不做任何锁动作
 */
class TC_EmptyMutex
{
public:
    /**
    * 写锁
    * @return int, 0 正确
    */
    int lock()  const   {return 0;}

    /**
    * 解写锁
    */
    int unlock() const  {return 0;}

    /**
    * 尝试解锁
    * @return int, 0 正确
    */
    bool trylock() const {return true;}
};

};
#endif

