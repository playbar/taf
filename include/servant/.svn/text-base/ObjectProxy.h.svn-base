#ifndef __TAF_OBJECT_PROXY_H_
#define __TAF_OBJECT_PROXY_H_

#include "servant/Communicator.h"
#include "servant/Message.h"
#include "servant/AdapterProxy.h"
#include "servant/EndpointInfo.h"
#include "servant/EndpointF.h"
#include "servant/BaseProxy.h"
#include "servant/Global.h"

namespace taf
{
class QueryEndpointImp;

/**
 * 用于保存服务端的一组列表
 * (按服务端灰度状态进行分组)
 */
struct AdapterProxyGroup
{
    AdapterProxyGroup();

    vector<AdapterPrx> adapterProxys;

    int32_t lastRoundPosition;

    AdapterPrx getHashProxy(int64_t hashCode);

    AdapterPrx getRandomProxy();

    AdapterPrx getNextValidProxy();
};
///////////////////////////////////////////////////////////////////
/**
 * 每个objectname在进程空间唯一有一个objectproxy
 * 管理收发的消息队列
 */
class ObjectProxy : public BaseProxy
{
public:
    /**
     * 构造函数
     * @param comm
     * @param name
     */
    ObjectProxy(Communicator* comm, const string& name);

    /**
     * 析构函数
     */
    ~ObjectProxy();

public:
    /**
     * 方法调用
     * @param req
     * @return int
     */
    int invoke(ReqMessagePtr& req);

    /**
     * 方法调用返回
     * @param req
     * @return int
     */
    int finished(ReqMessagePtr& req);

public:
    /**
     * 初始化消息队列等
     */
    void initialize();

    /**
     * 从队列中获取一个请求
     * @param req
     * @return bool
     */
    bool popRequest(ReqMessagePtr& req);

public:
    /**
     * 获取object名称
     * @return const string&
     */
    const string& name() const;

    /**
     * 获取消息队列超时，超时后的消息被丢弃
     * @return int
     */
    int timeout() const;

    /**
     * 设置消息队列超时，超时后的消息被丢弃
     * @param msec
     */
    void timeout(int msec);

    /**
     * 设置到单台server的连接数
     * @param num
     */
    void setAdapterMaxTransNum(int num);

    /**
     * 设置到单台server的连接数
     * @return int
     */
    int getAdapterMaxTransNum() const;

    /**
     * 超时策略获取和设置 
     * @return CheckTimeoutInfo& 
     */
    CheckTimeoutInfo& checkTimeoutInfo();

    /**
     * 设置协议解析器
     * @return UserProtocol& 
     */
    void setProxyProtocol(const ProxyProtocol& protocol);

    /**
     * 获取协议解析器
     * @return ProxyProtocol& 
     */
    ProxyProtocol& getProxyProtocol();

    /**
     * 获取可用服务列表 如果启用分组,只返回同分组的服务端ip 
     * @return vector<TC_Endpoint>
     */
    vector<TC_Endpoint> getEndpoint();

    /**
     * 获取所有服务列表  包括所有IDC
     * @return vector<TC_Endpoint>
     */
    vector<TC_Endpoint> getEndpoint4All();

	/**
	 * 获取所有服务列表 包括指定归属地 
	 *  @return void
	 */
	void getEndpoint4All(const std::string & sStation, vector<TC_Endpoint> & vecActive, vector<TC_Endpoint> & vecInactive);

    /**
     * 获取刷新服务列表的间隔
     * @return int
     */
    int refreshEndpointInterval() const;

    /**
     * 设置刷新服务列表的间隔
     * @param msecond
     */
    void refreshEndpointInterval(int msecond);
    
     /**
     * 设置缓存服务列表的间隔
     * @param msecond
     */
    void cacheEndpointInterval(int msecond);

    /**
     * 返回超时队列对象
     */
    TC_TimeoutQueue<ReqMessagePtr>* getTimeoutQueue();

    /**
     * 重置AdapterProxy的灰度状态
     * @param gridFrom
     * @param gridTo
     * @param adapter
     * @return bool
     */
    bool resetAdapterGrid(int32_t gridFrom, int32_t gridTo, AdapterProxy* adapter);

    /**
     * 加载locator
     * @param
     * @return AdapterPrx
     */
    int loadLocator();

    /**
     * 设置PUSH类消息的callback对象 
     * @param cb 
     */
    void setPushCallbacks(const ServantProxyCallbackPtr& cb);

    /**
     * 获取PUSH类消息的callback对象 
     */
    ServantProxyCallback* getOneRandomPushCallback();
  
protected:
    /**
     * 定时刷新服务列表（由registry）
     */
    void refreshEndpointInfos();

    /**
     * 选取一个AdapterProxy
     * @param req
     * @return AdapterPrx
     */
    AdapterPrx selectAdapterProxy(const ReqMessagePtr& req);

    /**
     * 轮询选取
     * @param adapters
     * @return AdapterPrx
     */
    AdapterPrx selectFromGroup(const ReqMessagePtr& req, AdapterProxyGroup& group);

private:
    string _name;
    
    string _locator;

    int32_t _timeout;

    int32_t _maxTransNum;

    string _queryFObjectName;

    bool _isDirectProxy;

    bool _serverHasGrid;

    time_t _lastRefreshEndpointTime;

    int32_t _refreshEndpointInterval;
    
    time_t _lastCacheEndpointTime;
    
    int32_t _cacheEndpointInterval;
     
    int32_t _roundStartIndex;

    TC_TimeoutQueue<ReqMessagePtr>* _timeoutQueue;

    ServantProxyCallback* _queryEndpoint;

    set<EndpointInfo> _activeEndpoints; //存活节点

    map<int32_t, AdapterProxyGroup> _adapterGroups;

    CheckTimeoutInfo _checkTimeoutInfo;

    ProxyProtocol _proxyProtocol;

    vector<ServantProxyCallbackPtr> _pushCallbacks;

private:
    int32_t _lastRefreshEndpoint4AllTime;
 
    set<EndpointInfo> _activeEndpoints4All; //所有存活节点 包括不在同IDC的

	std::map<std::string, std::pair<std::vector<TC_Endpoint>, std::vector<TC_Endpoint> > > _mapEndpoints;//缓存不同归属地的节点
};
///////////////////////////////////////////////////////////////////////////////////
}
#endif
