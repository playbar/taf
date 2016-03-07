#ifndef __TAF_FD_REACTOR_H_
#define __TAF_FD_REACTOR_H_

#include "servant/Global.h"
#include "servant/Transceiver.h"
#include "util/tc_thread.h"
#include "util/tc_thread_mutex.h"
#include "util/tc_epoller.h"
#include <set>

namespace taf
{
class TransceiverHandle;
/**
 * 监听FD事件并触发注册的handle
 */
class FDReactor : public TC_Thread, public TC_ThreadLock, public TC_HandleBase
{
public:
    /**
     * 构造函数
     */
    FDReactor();

    /**
     * 析构函数
     */
    virtual ~FDReactor();

public:
    /**
     * 循环监听网络事件
     */
    virtual void run();

    /**
     * 注册fd对应的处理handle
     * @param fd
     * @param event
     * @param handle
     */
    virtual void registerHandle(int fd, uint32_t event, TransceiverHandle* handle);

    /**
     * 取消已注册的handle
     * @param fd
     * @param event
     * @param handle
     */
    virtual void unregisterHandle(int fd, uint32_t event, TransceiverHandle* handle);

    /**
     * 中止监听
     */
    virtual void terminate();

    /**
     * 通知事件过来
     * @param fd
     */
    void notify(int fd);

protected:
    void handle(int fd, int events);

protected:
    TC_Socket _shutdown;

    bool _terminate;

    TC_Epoller _ep;

    map<int, TransceiverHandle*> _handles;

    //set<int>    _sfd;
};
/////////////////////////////////////////////////////////////////////////////////////
}

#endif
