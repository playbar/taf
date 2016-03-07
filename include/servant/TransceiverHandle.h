#ifndef __TAF_TRANSCEIVER_HANDLE_H_
#define __TAF_TRANSCEIVER_HANDLE_H_

#include "servant/AdapterProxy.h"
#include "servant/Transceiver.h"
#include "servant/Global.h"
#include "util/tc_thread.h"
#include "util/tc_thread_queue.h"

namespace taf
{
class FDReactor;

//////////////////////////////////////////////////////
/**
 * 处理网络事件
 */
class TransceiverHandle : public TC_HandleBase, public TC_ThreadMutex//, public TC_Thread
{
public:
    enum { R = 0x01, W = 0x02, };

    /**
     * 附属代理信息
     * 类内部使用
     */
    struct ProxyInfo
    {
        int event;      //输入事件 or 输出事件

        int fd;

        TransceiverPtr trans;

        AdapterPrx adapter;

        ProxyInfo() : event(-1), fd(-1), trans(NULL), adapter(NULL) {}
    };

public:
    /**
     * 构造函数
     * @param comm
     */
    TransceiverHandle(Communicator* comm);

    /**
     * 析构函数
     */
    virtual ~TransceiverHandle();

public:

    /**
     * 
     * @param fdReactor
     */
    void setFDReactor(FDReactor *fdReactor);

    /**
     * 注册一个连接附属的adapter
     * @param fd
     * @param trans
     * @param adapter
     */
    void registerProxy(int fd, TransceiverPtr& trans, const AdapterPrx& adapter);

    /**
     * 事件来了
     * @param fd
     * @param ev
     */
    void handle(int fd, int ev);

    /**
     * 有数据需要写
     * @param fd
     */
    void handleWrite(int fd);

    /**
     * 处理异常
     * @param fd
     * 
     * @return int
     */
    int handleExcept(int fd);

    /**
     * 中止handle
     */
//    void terminate();

protected:
//    void run();

    /**
     * 输入事件
     * @param pi
     */
    void handleInputImp(TransceiverHandle::ProxyInfo& pi);

    /**
     * 输出事件
     * @param pi
     */
    void handleOutputImp(TransceiverHandle::ProxyInfo& pi);

protected:
//    bool _bTerminate;

    Communicator* _comm;

    map<int, TransceiverHandle::ProxyInfo> _proxyInfo;

    FDReactor       *_fdReactor;

//    TC_ThreadQueue<ProxyInfo> _eventQueue;
};
//////////////////////////////////////////////////////
}
#endif
