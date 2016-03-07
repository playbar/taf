#ifndef __TAF_SERVANT_HANDLE_H
#define __TAF_SERVANT_HANDLE_H

#include <map>
#include <string>
#include <memory>
#include <deque>
#include "util/tc_monitor.h"
#include "util/tc_epoll_server.h"
#include "servant/Servant.h"
#include "servant/StatReport.h"
#include <ucontext.h>

namespace taf
{
//////////////////////////////////////////////////////////////////////////////
/**
 * 处理网络请求线程
 */
class ServantHandle : public TC_EpollServer::Handle
{
public:
    /**
     * 定义常量
     */
    enum
    {
        HEART_BEAT_INTERVAL = 10, /**心跳间隔时间**/
    };
   
    /**
     * 构造
     */
    ServantHandle();

    /**
     * 析够
     */
    ~ServantHandle();

protected:
    /**
     * 线程初始化
     */
    void initialize();

    /**
     * 逻辑处理
     * @param stRecvData
     */
    virtual void handle(const TC_EpollServer::tagRecvData &stRecvData);

    /**
     * 超时处理
     * @param stRecvData
     */
    virtual void handleTimeout(const TC_EpollServer::tagRecvData &stRecvData);

    /**
     * overload 处理
     * @param stRecvData
     */
    virtual void handleOverload(const TC_EpollServer::tagRecvData &stRecvData);

    /**
     * close 事件处理
     * @param stRecvData
     */
    virtual void handleClose(const TC_EpollServer::tagRecvData &stRecvData);

    /**
     * 处理Filter的消息
     */
    virtual void handleFilter();

    /**
     * 心跳
     */
    virtual void heartbeat();

    /**
     * 检查servant有没有resp消息待处理
     * @return bool 
     */
    virtual bool allFilterIsEmpty();

    /**
     * 创建上下文
     * @param stRecvData
     * @return JceCurrent*
     */
    JceCurrentPtr createCurrent(const TC_EpollServer::tagRecvData &stRecvData);

    /**
     * 处理Taf协议
     * 
     * @param current 
     */
    void handleTafProtocol(const JceCurrentPtr &current);

    /**
     * 处理非Taf协议
     * 
     * @param current 
     */
    void handleNoTafProtocol(const JceCurrentPtr &current);

    /**
     * 处理TAF下的灰度逻辑
     * 
     * @param current 
     */
    bool processGrid(const JceCurrentPtr &current);

    /**
     * 处理TAF下的染色逻辑
     * 
     * @param current 
     */
    bool processDye(const JceCurrentPtr &current);

protected:

    /**
     * 处理对象
     */
    map<string, ServantPtr> _servants;

    /**
     * 消息到达通知
     */
    TC_ThreadLock _monitor;
};

typedef TC_AutoPtr<ServantHandle> ServantHandlePtr;
///////////////////////////////////////////////////////////
}
#endif

