#ifndef __TC_THREAD_POOL_H_
#define __TC_THREAD_POOL_H_

#include "util/tc_thread.h"
#include "util/tc_thread_queue.h"
#include "util/tc_monitor.h"
#include "util/tc_functor.h"

#include <vector>
#include <set>
#include <iostream>

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: 线程池类,借鉴loki以及wbl的思想
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
* 线程异常
*/
struct TC_ThreadPool_Exception : public TC_Exception
{
	TC_ThreadPool_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_ThreadPool_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_ThreadPool_Exception() throw(){};
};


/**
 * 用通线程池类, 与tc_functor, tc_functorwrapper配合使用.
 * 使用方式说明:
 * 1 采用tc_functorwrapper封装一个调用
 * 2 用tc_threadpool对调用进行执行
 *
 * 具体示例代码请参见:test/test_tc_thread_pool.cpp
 */
class TC_ThreadPool : public TC_ThreadLock
{
public:

    /**
     * 构造函数
     *
     */
    TC_ThreadPool();

    /**
     * 析构, 会停止所有线程
     */
    ~TC_ThreadPool();

    /**
     * 初始化
     * @param num, 工作线程个数
     */
    void init(size_t num);

    /**
     * 获取线程个数
     *
     * @return size_t
     */
    size_t getThreadNum()   { Lock sync(*this); return _jobthread.size(); }

    /**
     * 获取线程池的任务数(exec添加进去的)
     *
     * @return size_t
     */
    size_t getJobNum()     { return _jobqueue.size(); }

    /**
     * 停止所有线程, 等待所有线程停止
     */
    void stop();

    /**
     * 启动所有线程
     */
    void start();

    /**
     * 启动所有线程并, 执行初始化对象
     * @param ParentFunctor
     * @param tf
     */
    template<class ParentFunctor>
    void start(const TC_FunctorWrapper<ParentFunctor> &tf)
    {
        for(size_t i = 0; i < _jobthread.size(); i++)
        {
            _startqueue.push_back(new TC_FunctorWrapper<ParentFunctor>(tf));
        }

        start();
    }

    /**
     * 添加对象到线程池执行
     * 该函数马上返回
     * 线程池的线程执行对象
	 */
    template<class ParentFunctor>
	void exec(const TC_FunctorWrapper<ParentFunctor> &tf)
    {
        _jobqueue.push_back(new TC_FunctorWrapper<ParentFunctor>(tf));
    }

    /**
     * 等待所有工作全部结束(队列无任务, 无空闲线程)
     *
     * @param millsecond, 等待的时间(ms), -1:永远等待
     * @return true, 所有工作都处理完毕
     *         false,超时退出
     */
    bool waitForAllDone(int millsecond = -1);

public:

    /**
     * 线程数据基类,所有线程的私有数据继承于该类
     */
    class ThreadData
    {
    public:
        /**
         * 构造
         */
        ThreadData(){};
        /**
         * 析够
         */
        virtual ~ThreadData(){};

        /**
         * 生成数据
         * @param T
         *
         * @return ThreadData*
         */
        template<typename T>
        static T* makeThreadData()
        {
            return new T;
        }
    };

    /**
     * 设置线程数据
     * @param p
     */
    static void setThreadData(ThreadData *p);

    /**
     * 获取线程数据
     *
     * @return ThreadData*
     */
    static ThreadData* getThreadData();

    /**
     * 设置线程数据, key需要自己维护
     * @param pkey, 线程私有数据key
     * @param p
     */
    static void setThreadData(pthread_key_t pkey, ThreadData *p);

    /**
     * 获取线程数据, key需要自己维护
     * @param pkey, 线程私有数据key
     * @return ThreadData*
     */
    static ThreadData* getThreadData(pthread_key_t pkey);

protected:

    /**
     * 释放资源
     * @param p
     */
    static void destructor(void *p);

    /**
     * 初始化key
     */
    class KeyInitialize
    {
    public:
        /**
         * 初始化key
         */
        KeyInitialize()
        {
            int ret = pthread_key_create(&TC_ThreadPool::g_key, TC_ThreadPool::destructor);
            if(ret != 0)
            {
                throw TC_ThreadPool_Exception("[TC_ThreadPool::KeyInitialize] pthread_key_create error", ret);
            }
        }

        /**
         * 释放key
         */
        ~KeyInitialize()
        {
            pthread_key_delete(TC_ThreadPool::g_key);
        }
    };

    /**
     * 初始化key的控制
     */
    static KeyInitialize g_key_initialize;

    /**
     * 数据key
     */
    static pthread_key_t g_key;

protected:
    /**
     * 线程池中的工作线程
     */
    class ThreadWorker : public TC_Thread
    {
    public:
        /**
         * 工作线程构造函数
         * @param tpool
         */
        ThreadWorker(TC_ThreadPool *tpool);

        /**
         * 通知工作线程结束
         */
        void terminate();

    protected:
        /**
         * 运行
         */
        virtual void run();

    protected:
        /**
         * 线程池指针
         */
        TC_ThreadPool   *_tpool;

        /**
         * 是否结束线程
         */
        bool            _bTerminate;
    };

protected:

    /**
     * 清除
     */
    void clear();

    /**
     * 获取任务, 如果没有任务, 则为NULL
     *
     * @return TC_FunctorWrapperInterface*
     */
    TC_FunctorWrapperInterface *get(ThreadWorker *ptw);

    /**
     * 获取启动任务
     *
     * @return TC_FunctorWrapperInterface*
     */
    TC_FunctorWrapperInterface *get();

    /**
     * 空闲了一个线程
     * @param ptw
     */
    void idle(ThreadWorker *ptw);

    /**
     * 通知等待在任务队列上的工作线程醒来
     */
    void notifyT();

    /**
     * 是否处理结束
     *
     * @return bool
     */
    bool finish();

    /**
     * 线程退出时调用
     */
    void exit();

    friend class ThreadWorker;
protected:

    /**
     * 任务队列
     */
    TC_ThreadQueue<TC_FunctorWrapperInterface*> _jobqueue;

    /**
     * 启动任务
     */
    TC_ThreadQueue<TC_FunctorWrapperInterface*> _startqueue;

    /**
     * 工作线程
     */
    std::vector<ThreadWorker*>                  _jobthread;

    /**
     * 繁忙线程
     */
    std::set<ThreadWorker*>                     _busthread;

    /**
     * 任务队列的锁
     */
    TC_ThreadLock                               _tmutex;

};

}
#endif

