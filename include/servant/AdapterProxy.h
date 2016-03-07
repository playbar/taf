#ifndef __TAF_ADAPTER_PROXY_H_
#define __TAF_ADAPTER_PROXY_H_

#include "util/tc_timeout_queue.h"
#include "servant/TransceiverHandle.h"
#include "servant/EndpointInfo.h"
#include "servant/Connector.h"
#include "servant/ObjectProxy.h"
#include "servant/TimeoutHandle.h"
#include "servant/FDReactor.h"
#include "servant/Transceiver.h"
#include "servant/BaseProxy.h"
#include "servant/Global.h"

namespace taf
{
/**
 * 每个Adapter对应一个Endpoint，也就是一个服务端口
 */
class AdapterProxy : public BaseProxy
{
public:
    /**
     * 构造函数
     * @param ep
     * @param op
     * @param rt
     * @param transHandle
     */
    AdapterProxy(Communicator* comm, const EndpointInfo &ep, ObjectProxy* op, FDReactor* rt, TransceiverHandle* transHandle);

    /**
     * 析构函数
     */
    ~AdapterProxy();

public:
    /**
     * 调用server端对象方法
     * @param req
     * @return int
     */
	int invoke(ReqMessagePtr& req);

    /**
     * server端方法返回
     * @param req
     * @return int
     */
    int finished(ReqMessagePtr& req);

public:
    /**
     * 端口是否有效,当连接全部失效时返回false
     * @return bool
     */
    bool checkActive();

    /**
     * 设置服务的有效状态
     * @param value
     */
    void setActive(bool value);

    /**
     * 重新设置信息
     */
    void resetInvoke();

    /**
     * 增加调用数
     * @param bTimeout, 是否是超时的
     */
    void finishInvoke(bool bTimeout);

    /**
     * 获取端口信息
     * @return const EndpointInfo&
     */
    const EndpointInfo& endpoint();

    /**
     * 从队列里取一个请求
     * @param trans
     * @return bool
     */
	bool sendRequest(TransceiverPtr& trans);

    /**
     * 定时检查trans
     */
    void refreshTransceiver();

    /**
     * 处理没有找到request的response(可能是push消息)
     * @param req 
     * @return bool 
     */
    bool dispatchPushCallback(ReqMessagePtr& req);

protected:
    /**
     * 从队列中获取一个请求
     * @param req
     * @return bool
     */
	bool popRequest(ReqMessagePtr& req);

    /**
     * 选取一个连接
     * @param trans
     * @return TransceiverPtr
     */
    TransceiverPtr selectTransceiver();

    /**
     * 重连
     * @return TransceiverPtr
     */
    TransceiverPtr doReconnect();

private:
    EndpointInfo _endpoint;

    ObjectProxy* _objectProxy;

	FDReactor* _reactor;

    TransceiverHandle* _transHandle;

    TC_TimeoutQueue<ReqMessagePtr>* _timeoutQueue;

	vector<TransceiverPtr> _trans;

    int32_t _checkTransInterval;

    time_t _lastCheckTransTime;

    int32_t _currentGridGroup;

    int32_t _connectTimeout;

    uint32_t _timeoutInvoke;

    uint32_t _totalInvoke;

    uint32_t _frequenceFailInvoke;

    time_t _lastFinishInvokeTime;

    time_t _lastRetryTime;

    bool _activeStatus;
};
////////////////////////////////////////////////////////////////////
}
#endif
