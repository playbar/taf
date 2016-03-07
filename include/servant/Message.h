#ifndef __TAF_MESSAGE_H_
#define __TAF_MESSAGE_H_

#include "servant/Global.h"
#include "servant/EndpointInfo.h"
#include "util/tc_autoptr.h"
#include "util/tc_monitor.h"

namespace taf
{
/**
 * 用于同步调用时的条件变量
 */
struct ReqMonitor : public TC_ThreadLock, public TC_HandleBase {};

typedef TC_AutoPtr<ReqMonitor> ReqMonitorPtr;

#define IS_MSG_TYPE(m, t) ((m & t) != 0)
#define SET_MSG_TYPE(m, t) do { (m |= t); } while (0);
#define CLR_MSG_TYPE(m, t) do { (m &=~t); } while (0);

/**
 * Proxy端的请求消息结构
 */
struct ReqMessage : public TC_HandleBase//, public TC_ThreadPool::ThreadData
{
    //调用类型
    enum CallType
    {
        SYNC_CALL = 1, //同步
        ASYNC_CALL,    //异步
        ONE_WAY,       //单向
    };
    /**
     * 构造函数
     */
    ReqMessage();
    /**
     * 析构函数
     */
    ~ReqMessage();
public:
    /**
     * 判断消息类型
     * 
     * @return bool
     */
    bool is_message_type(uint32_t msg, uint32_t type);
    /**
     * 
     * @param msg
     * @param type
     */
    void set_message_type(uint32_t& msg, uint32_t type);
    /**
     * 
     * @param msg
     * @param type
     */
    void clr_message_type(uint32_t& msg, uint32_t type);

public:
    ReqMessage::CallType        type;       //调用类型
    ServantPrx::element_type*   proxy;      //调用端的proxy对象
    ReqMonitorPtr               monitor;    //用于同步的monitor
    AdapterPrx                  adapter;    //哪台服务器发送的
    RequestPacket               request;    //请求消息体
    ResponsePacket              response;   //响应消息体
    ServantProxyCallbackPtr     callback;   //异步调用时的回调对象
    EndpointInfo                endpoint;   //被请求到的服务地址
    timeval                     begtime;    //请求时间
    timeval                     endtime;    //完成时间
    int64_t                     hashCode;   //hash值，用户保证一个用户的请求发送到同一个server(不严格保证)
    bool                        fromRpc;    //是否是第三方协议的rcp_call，缺省为false
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}
#include "servant/ServantProxy.h"

#endif
