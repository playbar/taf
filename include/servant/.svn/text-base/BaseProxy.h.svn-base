#ifndef __TAF_BASE_PROXY_H_
#define __TAF_BASE_PROXY_H_

#include "servant/Global.h"
#include "servant/AppProtocol.h"

namespace taf
{
/**
 * 超时一定比率后进行切换
 * 设置超时检查参数
 * 计算到某台服务器的超时率, 如果连续超时次数或者超时比例超过阀值
 * 默认60s内, 超时调用次数>=2, 超时比率0.7或者连续超时次数>500,
 * 则失效
 * 服务失效后, 请求将尽可能的切换到其他可能的服务器, 并每隔tryTimeInterval尝试一次, 如果成功则认为恢复
 * 如果其他服务器都失效, 则随机选择一台尝试
 * 如果是灰度状态的服务, 服务失效后如果请求切换, 也只会转发到相同灰度状态的服务
 * @uint16_t minTimeoutInvoke, 计算的最小的超时次数, 默认2次(在checkTimeoutInterval时间内超过了minTimeoutInvoke, 才计算超时)
 * @uint32_t frequenceFailInvoke, 连续失败次数
 * @uint32_t checkTimeoutInterval, 统计时间间隔, (默认60s, 不能小于30s)
 * @float radio, 超时比例 > 该值则认为超时了 ( 0.1<=radio<=1.0 )
 * @uint32_t tryTimeInterval, 重试时间间隔
 */
struct CheckTimeoutInfo
{
    uint16_t minTimeoutInvoke;

    uint32_t checkTimeoutInterval;

    uint32_t frequenceFailInvoke;

    float radio;

    uint32_t tryTimeInterval;

    CheckTimeoutInfo() : minTimeoutInvoke(2), checkTimeoutInterval(60), frequenceFailInvoke(10), radio(0.9), tryTimeInterval(30) {}
};
/**
 * Proxy的基类
 */
class BaseProxy : public TC_HandleBase, public TC_ThreadMutex
{
public:
    /**
     * 构造函数
     */
    BaseProxy(Communicator* comm);

    /**
     * 析构函数
     */
    virtual ~BaseProxy();

    /**
     * 调用方法
     * @param req
     * @return int
     */
    virtual int invoke(ReqMessagePtr& req) = 0;

    /**
     * 服务端响应
     * @param req
     * @return int
     */
    virtual int finished(ReqMessagePtr& req) = 0;

    /**
     * @return Communicator*
     */
    Communicator* getCommunicator();

    /**
     * 上报模块间调用
     * @param req
     */
    void reportToStat(ReqMessagePtr& req, const string& obj, int result);
    
    /**
     * 上报模块间调用 采样
     * @param req
     */
    void sampleToStat(ReqMessagePtr& req, const string& obj,const string &ip);
    
protected:
    Communicator* _comm;
};
/////////////////////////////////////////////////////////////////////
}
#endif
