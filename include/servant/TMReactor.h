#ifndef __TAF_TM_REACTOR_H_
#define __TAF_TM_REACTOR_H_

#include "servant/Global.h"
#include "servant/TMEventHandle.h"
#include "util/tc_thread.h"
#include "util/tc_monitor.h"
#include "util/tc_epoller.h"

namespace taf
{
/**
 * 定时事件的触发器
 */
class TMReactor : public TC_Thread, public TC_ThreadLock, public TC_HandleBase
{
public:
    /**
     * 定时信息
     */
    struct TimerInfo
    {
        uint32_t timeout;       //定时周期

        time_t lastHandleTime;  //上次触发时间
    };

public:
    /**
     * 构造函数
     */
    TMReactor();

    /**
     * 析构函数
     */
    virtual ~TMReactor();

public:
    /**
     * 主线程，循环检查时间
     */
    virtual void run();

    /**
     * 注册事件处理器
     * @param timeout
     * @param handle
     */
    virtual void registerHandle(int timeout, TMEventHandle* handle);

    /**
     * 注销事件处理器
     * @param timeout
     * @param handle
     */
    virtual void unregisterHandle(int timeout, TMEventHandle* handle);

    /**
     * 结束处理
     */
    virtual void terminate();

protected:
    bool _terminate;

    map<TMEventHandle*, TimerInfo> _handles;
};
////////////////////////////////////////////////////
}
#endif
