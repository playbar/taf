#ifndef __TAF_SERVANT_H_
#define __TAF_SERVANT_H_

#include "util/tc_autoptr.h"
#include "util/tc_epoll_server.h"
#include "servant/ServantProxy.h"
#include "servant/JceCurrent.h"
#include "servant/BaseNotify.h"

namespace taf
{

class BaseNotify;

/**
 * 每个对象的基类
 */
class Servant : public BaseNotify
{
public:
    /**
     * 构造函数
     */
    Servant();

    /**
     * 析构函数
     */
    ~Servant();

    /**
     * 设置名称
     * @param name
     */
    void setName(const string &name);

    /**
     * 名称
     * @return string
     */
    string getName() const;

    /**
     * 设置所属的Handle
     * @param handle 
     */
    void setHandle(TC_EpollServer::Handle* handle);

    /**
     * 获取所属的Handle
     * @return HandlePtr& 
     */
    TC_EpollServer::Handle* getHandle();

    /**
     * 初始化
     * @return int
     */
    virtual void initialize() = 0;

    /**
     * 退出
     */
    virtual void destroy() = 0;

public:
    /**
     * 分发收到的请求
     * @param sRecv
     * @param sFuncName
     * @param sSend
     * @return int
     */
    virtual int dispatch(JceCurrentPtr current, vector<char> &buffer);

    /**
     * taf协议，分发并处理请求
     * @param sRecv
     * @param sFuncName
     * @param sSend
     * @return int
     */
    virtual int onDispatch(JceCurrentPtr current, vector<char> &buffer) { return -1; }


	/*
	socket关闭后，会回调该函数
	*/
	virtual int handleClose(JceCurrentPtr current, vector<char> &buffer){ return -1; }

public:
    /**
     * 普通协议的请求，没有方法名，不需要Dispatch
     * @param current 
     * @param buffer 
     * @return int 
     */
    virtual int doRequest(JceCurrentPtr current, vector<char> &buffer) { return -1; }

    /**
     * 作为客户端访问其他server时，成功返回的响应接口
     * @param resp 
     * @return int 
     */
    virtual int doResponse(ReqMessagePtr resp) { return -1; }

    /**
     * 作为客户端访问其他server时，返回其他异常的响应接口
     * @param resp 
     * @return int 
     */
    virtual int doResponseException(ReqMessagePtr resp) { return -1; }

    /**
     * 作为客户端访问其他server时，如果resp没有找到request,则响应该接口
     * 例如:push消息或者超时后服务端迟到的响应
     * @param resp 
     * @return int 
     */
    virtual int doResponseNoRequest(ReqMessagePtr resp) { return -1; }

    /**
     * 每次handle被唤醒后都会调用，业务可以通过在其他线程中调用handle的notify
     * 实现在主线程中处理自有数据的功能，比如定时器任务或自有网络的异步响应;
     * [一般都需要配合业务自有的队列使用，队列可以封装在ServantImp对象中]
     */
    virtual int doCustomMessage() { return -1; }

    /**
     * 获得响应的数据队列
     * @return TC_ThreadQueue<ReqMessagePtr>& 
     */
    TC_ThreadQueue<ReqMessagePtr>& getResponseQueue();

public:


    /**
     * 处理服务端路由逻辑
     * @param current
     * @param routekey
     * @return int 0成功路由
     */
    virtual int doGridRouter(JceCurrentPtr current, string & routekey);


protected:
    /**
     * 名字
     */
    string _name;

    /**
     * 所属的Handle
     */
    TC_EpollServer::Handle* _handle;

    /**
     * 异步响应队列
     */
    TC_ThreadQueue<ReqMessagePtr> _asyncResponseQueue;
};

typedef TC_AutoPtr<Servant> ServantPtr;
//////////////////////////////////////////////////////////////////////
/**
 * 单线程全异步中的callback对象，业务自有的callback需要从这里继承
 */
class ServantCallback : public ServantProxyCallback
{
public:
    /**
     * 构造函数，type用来区分同一链路上的多种cb对象类型
     * @param type 
     * @param servant 
     * @param current 
     */
    ServantCallback(const string& type, const ServantPtr& servant, const JceCurrentPtr& current);

    /**
     * callback的响应接口 
     * @param msg 
     * @return int 
     */
    virtual int onDispatch(ReqMessagePtr msg);

    /**
     * 获得生成时所属的servant
     * @return const ServantPtr& 
     */
    const ServantPtr& getServant();

    /**
     * 获得网络上下文
     * @return const JceCurrentPtr& 
     */
    const JceCurrentPtr& getCurrent();

protected:
    ServantPtr _servant;

    JceCurrentPtr _current;
};

//////////////////////////////////////////////////////////////////////
/**
 * 服务端启用路由 返回的callback
 */
class RouterCallback : public ServantCallback
{
public:
    RouterCallback(const string& type, const ServantPtr& servant, const JceCurrentPtr& current);
    virtual int onDispatch(ReqMessagePtr msg);
};

typedef TC_AutoPtr<ServantCallback> ServantCallbackPtr; 
//////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////
#endif
