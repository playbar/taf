#ifndef	__TC_THREAD_H_
#define __TC_THREAD_H_

#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include "util/tc_ex.h"
#include "util/tc_monitor.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 线程类(修改至ICE源码)
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
 * 线程控制异常类
 */
struct TC_ThreadThreadControl_Exception : public TC_Exception
{
    TC_ThreadThreadControl_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_ThreadThreadControl_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_ThreadThreadControl_Exception() throw() {};
};

/**
 * 线程控制类
 */
class TC_ThreadControl
{
public:

    /**
     * 构造, 表示当前运行的线程
     * join和detach在不能在该对象上调用
     */
    TC_ThreadControl();

    /**
     *
     *
     * @return explicit
     */
    explicit TC_ThreadControl(pthread_t);

    /**
     * 等待当前线程结束, 不能在当前线程上调用
     */
    void join();

    /**
     * detach, 不能在当前线程上调用
     */
    void detach();

    /**
     * 当前线程id
     *
     * @return pthread_t
     */
    pthread_t id() const;

    /**
     * 休息ms时间
     * @param millsecond
     */
    static void sleep(long millsecond);

    /**
     * 交出当前线程控制权
     */
    static void yield();

private:

    pthread_t _thread;
};

/**
 *
 */
class TC_Runable
{
public:
    virtual ~TC_Runable(){};
    virtual void run() = 0;
};

/**
 * 线程基类
 */
class TC_Thread : public TC_Runable
{
public:

	/**
     * 构造函数
	 */
	TC_Thread();

	/**
     * 析构函数
	 */
	virtual ~TC_Thread(){};

	/**
     * 线程运行
	 */
	TC_ThreadControl start();

    /**
     * 获取线程控制类
     *
     * @return ThreadControl
     */
    TC_ThreadControl getThreadControl() const;

    /**
     * 线程是否存活
     *
     * @return bool
     */
    bool isAlive() const;

	/**
     * 获取线程id
	 *
	 * @return pthread_t
	 */
	pthread_t id() { return _tid; }

protected:

	/**
     * 静态函数, 线程入口
	 * @param pThread
	 */
	static void threadEntry(TC_Thread *pThread);

	/**
     * 运行
	 */
    virtual void run() = 0;

protected:
    /**
     * 是否在运行
     */
    bool            _running;

    /**
     * 线程ID
     */
	pthread_t	    _tid;

    /**
     * 线程锁
     */
    TC_ThreadLock   _lock;
};

}
#endif

