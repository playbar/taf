#ifndef __TAF_COMMUNICATOR_H_
#define __TAF_COMMUNICATOR_H_

#include "util/tc_thread.h"
#include "util/tc_config.h"
#include "util/tc_singleton.h"
#include "util/tc_timeout_queue.h"
#include "servant/Global.h"
#include "servant/ServantProxy.h"
#include "servant/ServantProxyFactory.h"
#include "servant/ObjectProxyFactory.h"
#include "servant/TransceiverHandle.h"
#include "servant/AsyncProcThread.h"
#include "servant/FDReactor.h"
#include "servant/TMReactor.h"
//#include "servant/TMEventHandle.h"
#include "servant/TimeoutHandle.h"

#define CONFIG_ROOT_PATH "/taf/application/client"

namespace taf
{
////////////////////////////////////////////////////////////////////////
/**
 * 客户端配置
 */
struct ClientConfig
{
    /**
     * 客户端IP地址
     */
    static string LocalIp;
    /**
     * 客户端模块名称
     */
    static string ModuleName;
};
////////////////////////////////////////////////////////////////////////
/**
 * 通信器,用于创建和维护客户端proxy
 */
class Communicator : public TC_HandleBase, public TC_ThreadRecMutex
{
public:
    /**
     * 构造函数
     */
    Communicator();

    /**
     * 采用配置构造
     * @param conf
     * @param path
     */
    Communicator(TC_Config& conf, const string& domain = CONFIG_ROOT_PATH);

    /**
     * 析够
     * 析够时自动接收相关线程
     */
    ~Communicator();

    /**
     * 生成代理
     * @param T
     * @param objectName
     * @return T
     */
    template<class T> T stringToProxy(const string& objectName)
    {
        T prx = NULL;

        stringToProxy<T>(objectName, prx);

        return prx;
    }

    /**
     * 生成代理
     * @param T
     * @param objectName
     * @param proxy
     */
    template<class T> void stringToProxy(const string& objectName, T& proxy)
    {
        ServantPrx prx = getServantProxy(objectName);

        proxy = (typename T::element_type*)(prx.get());
    }

public:
    /**
     * 获取属性
     * @param name
     * @param dft, 缺省值
     * @return string
     */
    string getProperty(const string& name, const string& dft = "");

    /**
     * 设置属性
     * @param properties
     */
    void setProperty(const map<string, string>& properties);

    /**
     * 设置某一个属性
     * @param name
     * @param value
     */
    void setProperty(const string& name, const string& value);

    /**
     * 设置属性
     * @param conf
     * @param path
     */
    void setProperty(TC_Config& conf, const string& domain = CONFIG_ROOT_PATH);

    /**
     * 上报统计
     * @return StatReport*
     */
    StatReport* getStatReport();

    /**
     * 重新加载属性
     */
    int reloadProperty(string & sResult);
    
    /**
     * 获取obj对应可用ip port列表  如果启用分组,只返回同分组的服务端ip 
     * @param sObjName
     * @return vector<TC_Endpoint>
     */
    vector<TC_Endpoint> getEndpoint(const string & objName);

    /**
     * 获取obj对应可用ip port列表 包括所有IDC的
     * @param sObjName
     * @return vector<TC_Endpoint>
     */
    vector<TC_Endpoint> getEndpoint4All(const string & objName);
    
protected:
    /**
     * 初始化
     */
    void initialize();

    /**
     * 结束
     */
    void terminate();

    /**
     * 是否析构中
     * @return bool
     */
    bool isTerminating();

    /**
     * 由Property初始化客户端配置
     */
    void initClientConfig();

    /**
     * 获取对象生成器
     * @return ObjectProxyFactoryPtr
     */
    ObjectProxyFactory* objectProxyFactory();

    /**
     * 获取对象代理生成器
     * @return ServantProxyFactoryPtr
     */
    ServantProxyFactory* servantProxyFactory();

    /**
     * 传输处理器
     * @return TransceiverHandlePtr
     */
    TransceiverHandle* transceiverHandle();

    /**
     * fd反应器
     * @return FDReactorPtr
     */
    FDReactor* fdReactor();

    /**
     * 获取异步处理线程
     * @return AsyncProcThreadPtr
     */
    AsyncProcThread* asyncProcThread();

    /**
     * 获取通用对象
     * @param objectName
     * @return ServantPrx
     */
    ServantPrx getServantProxy(const string& objectName);

    /**
     * 获取超时处理器
     * @return TimeoutHandle<TC_TimeoutQueue<ReqMessagePtr>>*
     */
    TimeoutHandle<TC_TimeoutQueue<ReqMessagePtr> >* getTimeoutHandle();

    /**
     * 框架内部需要直接访问通信器的类
     */
    friend class ReqMessage;

    friend class BaseProxy;

    friend class AdapterProxy;

    friend class ServantProxy;

    friend class ObjectProxy;

    friend class ServantProxyFactory;

    friend class ObjectProxyFactory;

protected:
    bool _initialized;

    bool _terminating;

    map<string, string> _properties;

    ObjectProxyFactoryPtr _objectProxyFactory;

    ServantProxyFactoryPtr _servantProxyFactory;

    TransceiverHandlePtr _transceiverHandle;

    TimeoutHandle<TC_TimeoutQueue<ReqMessagePtr> >* _timeoutHandle;

    StatReportPtr _statReportPtr;

    FDReactorPtr _fdReactor;

    TMReactorPtr _tmReactor;

    AsyncProcThreadPtr _asyncProcThread;
};
////////////////////////////////////////////////////////////////////////
}
#endif
