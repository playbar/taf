#ifndef __TAF_ASYNC_PROC_THREAD_H_
#define __TAF_ASYNC_PROC_THREAD_H_

#include "servant/Message.h"
#include "servant/Global.h"

namespace taf
{
/**
 * 异步回调后的业务处理线程，线程数在client配置
 */
///////////////////////////////////////////////////////
class AsyncProcThread : public TC_HandleBase
{
public:
    /**
     * 构造函数
     */
    AsyncProcThread(Communicator * pcomm);
    /**
     * 析构函数
     */
    virtual ~AsyncProcThread();

public:
    /**
     * 结束所有runner
     */
    void terminate();

    /**
     * 放入回调消息到队列，待执行
     * @param req
     */
    void put(const ReqMessagePtr& req);

    /**
     * 从队列头部取出一条待执行的消息
     * @param req
     * @return bool
     */
    bool pop(ReqMessagePtr& req);

    /**
     * 启动n个runner，每个runner一个线程
     */
    void start(int n);

protected:

    /**
     * 结束
     */
    bool _terminate;

    /**
     * 消息队列
     */
    TC_ThreadQueue<ReqMessagePtr> _queue;       

    /**
     * runner对象(每个对象维护一个线程)
     */
    vector<AsyncProcThreadRunnerPtr> _runners;  

	Communicator * _pComm;


	friend class AsyncProcThreadRunner;
};

///////////////////////////////////////////////////////
class AsyncProcThreadRunner : public TC_Thread, public TC_HandleBase
{
public:
    /**
     * 构造函数
     * @param proc
     */
    AsyncProcThreadRunner(AsyncProcThread* proc);

    /**
     * 结束处理线程
     */
    void terminate();

    /**
     * 从队列中取消息后执行回调逻辑
     */
    void run();

	/**
	 * 上报队列中的请求数
	 */
	void report();
private:
    /**
     * 是否需要退出
     */
    bool _terminate;

    /**
     * 问访消息队列的所有者
     */
    AsyncProcThread* _proc;

	/**
	 * 上报时间
	 */
	time_t _timeLastReport;
};
///////////////////////////////////////////////////////
}
#endif
