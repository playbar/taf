#ifndef _TAF_SERVANT_PROXY_H_
#define _TAF_SERVANT_PROXY_H_

#include "servant/Message.h"
#include "servant/BaseProxy.h"
#include "servant/JceCurrent.h"
#include "util/tc_monitor.h"
#include "util/tc_pack.h"
#include "util/tc_autoptr.h"

namespace taf
{

//////////////////////////////////////////////////////////////////////////
//tat采样信息(用于调用链时序分析)
struct SampleKey
{
    bool _root;   //是否根节点 在根节点产生唯一id

    string _unid; //唯一id  

    int _depth;   //深度

    int _width;   //广度

    int _parentWidth;  //父节点广度值
    
    SampleKey() : _root(true), _unid(""), _depth(0), _width(0), _parentWidth(0) {}
};

//////////////////////////////////////////////////////////////////////////
//线程的私有数据(无论TAF同步还是异步调用, 都会往后带)
struct ThreadPrivateData : virtual public TC_HandleBase
{
    static int INVALID_HASH_CODE;

    static int INVALID_GRID_CODE;

    int64_t   _hashCode;  //路由时的hash值

    int32_t   _gridCode;  //灰度状态值

    string    _routeKey;  //灰度路由的key值

    string    _dyeingKey; //染色的key值
    
    SampleKey _sampleKey; //采样的Key

	bool	  _bDyeing;	  //标识当前线程是否需要染色

    ThreadPrivateData() : _hashCode(INVALID_HASH_CODE), _gridCode(INVALID_GRID_CODE),_bDyeing(false) {}

    /**
     * 设置数据
     * @param data
     */
    void set(const string &name,const string &value);

    /**
     * 获取数据
     * @param data
     */
    bool get(const string &name,string &value) const;

private:
	TC_ThreadMutex _mutex;

    map<string,string> _dataMap; 
};

typedef TC_AutoPtr<ThreadPrivateData> ThreadPrivateDataPtr;

//////////////////////////////////////////////////////////////////////////
// 异步回调对象的基类
class ServantProxyCallback : virtual public TC_HandleBase
{
public:
    /**
     * 析构函数
     */
    virtual ~ServantProxyCallback() {}

    /**
     * 获取类型
     * @return const string& 
     */
    virtual const string& getType() { return _type; }

    /**
     * 设置类型
     * @return const string& 
     */
    virtual void setType(const string& type) { _type = type; }

public:
    /**
     * 异步回调对象实现该方法，进行业务逻辑处理
     * @param msg
     * @return int
     */
    virtual int onDispatch(ReqMessagePtr msg) = 0;

    /**
     * 线程私有数据
     */
    ThreadPrivateDataPtr _dataPtr;   

    /**
     * 同一链路多个cb的时候可以用来区分class类型
     */
    string _type;
};

//////////////////////////////////////////////////////////////////////////
//用户级线程的异步回调对象(采用异步模拟同步)
//////////////////////////////////////////////////////////////////////////
//线程私有数据
struct ServantProxyThreadData : public TC_ThreadPool::ThreadData
{
    static TC_ThreadMutex mutex;  //全局的互斥锁

    static pthread_key_t g_key;   //私有线程数据key

    /**
     * 数据资源释放
     * @param p
     */
    static void destructor(void* p);

    /**
     * 获取线程数据，没有的话会自动创建
     * @return ServantProxyThreadData*
     */
    static ServantProxyThreadData* getData();

    /**
     * 用户级线程时用到
     * @return ServantProxyThreadData*
     */
    static ServantProxyThreadData* makeData();

    /**
     * 构造函数
     */
    ServantProxyThreadData();

    /**
     * 获取线程私有数据
     * @return ThreadPrivateData&
     */
    const ThreadPrivateDataPtr & data();

    /**
     * 设置数据
     * @param data
     */
    void data(const ThreadPrivateDataPtr & dataPtr);

    /**
     * 业务的线程私有数据
     * @param p
     */
    void setPrivatePointer(void *p);

    /**
     * 获取业务的线程私有数据
     * @return void*
     */
    void* getPrivatePointer();

protected:
    void                    *_p;                        //业务线程私有数据

    ThreadPrivateDataPtr    _dataPtr;                   //用户私有数据    
};
//////////////////////////////////////////////////////////////////////////
/**
 * ServantProxy的路由器，由业务实现并注册到ServantProxy
 * 完整的切换逻辑如下：
 * -服务器根据灰度状态分组
 * -请求根据当前的用户灰度状态从分组中选择相应的服务器随机发送
 * -如果某台服务器超时超过一定值， 则将这台服务器设置为失效状态并关闭所有连接， 并定时重试
 * -具体超时的判断逻辑请参见taf_check_timeout说明
 */
struct ServantProxyRouter : public TC_HandleBase
{
    /**
     * 析构函数
     */
    virtual ~ServantProxyRouter() {}

    /**
     * 根据key获取用户的灰度状态，由业务实现，
     * 返回INVALID_GRID_CODE则认为无灰度状态
     * @param key
     * @return int
     */
    virtual int getGridByKey(const string& key) = 0;
};

//////////////////////////////////////////////////////////////////////////
/**
 * 1:远程对象的本地代理 
 * 2:同名servant在一个通信器中最多只有一个实例
 * 3:防止和用户在Jce中定义的函数名冲突，接口以taf_开头
 */
class ServantProxy : public BaseProxy
{
public:
    /**
     * 通过status传递数据时用到的缺省字符串
     */
    static string STATUS_DYED_KEY;  //需要染色的用户ID

    static string STATUS_GRID_KEY;  //需要路由的用户ID

    static string STATUS_GRID_CODE; //目标服务的灰度值
    
    static string STATUS_SAMPLE_KEY; //stat 采样的信息

    static string STATUS_RESULT_CODE; //处理结果码， wup使用

    static string STATUS_RESULT_DESC; //处理结果描述,wup使用

    /**
     * 缺省的同步调用超时时间
     * 超时后不保证消息不会被服务端处理
     */
    enum { DEFAULT_TIMEOUT = 3000, };

    /**
     * 构造函数
     * @param op
     */
    ServantProxy(Communicator* comm, ObjectProxy* op);

    /**
     * 析构函数
     */
    virtual ~ServantProxy();

public:
    /**
     * 构造函数
     */
    ServantProxy();

    /**
     * 发送测试消息到服务器
     */
    void taf_ping();

    /**
     * 设置同步调用超时时间，对该proxy上所有方法都有效
     * @param msecond
     */
    void taf_timeout(int msecond);

    /**
     * 获取同步调用超时时间，对该proxy上所有方法都有效
     * @return int
     */
    int taf_timeout() const;

    /**
     * 设置到单个Server的连接数
     * @param num
     */
    void taf_max_trans_num(int num);

    /**
     * 获取到单个Server的连接数
     * @return int
     */
    int taf_max_trans_num() const;

    /**
     * 获取所属的Object名称
     * @return string
     */
    string taf_name() const;
  
	/**
	 * 获取最近一次调用的IP地址和端口 
	 * @return string 
	 */
	static TC_Endpoint taf_invoke_endpoint();

     /**
     * 获取Object可用服务列表 如果启用分组,只返回同分组的服务端ip 
     * @return vector<TC_Endpoint>
     * 怎么没用 taf 开头-_-!!!!!!!
     */
    vector<TC_Endpoint> getEndpoint();

    /**
     * 获取Object可用服务列表 包括所有IDC
     * @return vector<TC_Endpoint>
     * 怎么没用 taf 开头-_-!!!!!!!
     */
    vector<TC_Endpoint> getEndpoint4All();

	/**
	 * 获取Object可用服务列表 包括指定归属地 
	 *  @return vector<TC_Endpoint>
	 */
	vector<TC_Endpoint> taf_endpoints(const std::string & sStation);

	/**
	 * 获取Object可用服务列表 包括指定归属地 
	 *  @return void
	 */
	void taf_endpoints(const std::string & sStation, vector<TC_Endpoint> & vecActive, vector<TC_Endpoint> & vecInactive);


    /**
     * 设置用户自定义协议
     * @param protocol 
     */
    void taf_set_protocol(const ProxyProtocol& protocol);

    /**
     * 获取和设置超时检查参数
     */
    CheckTimeoutInfo& taf_check_timeout();

    /**
     * hash方法，为保证一段时间内同一个key的消息发送
     * 到相同的服务端，由于服务列表动态变化，所以
     * 不严格保证
     * @param key
     * @return ServantProxy*
     */
    virtual ServantProxy* taf_hash(int64_t key);

    /**
     * 清除当前的Hash数据
     * @param key
     * @return ServantProxy*
     */
    virtual void taf_clear_hash();

    /**
     * 设置二级路由器
     * @param router
     */
    virtual void taf_set_router(const ServantProxyRouterPtr& router);

    /**
     * 清除当前的二级路由器
     */
    virtual void taf_del_router();

    /**
     * 用proxy产生一个该object上的序列号
     * @return uint32_t 
     */
    virtual uint32_t taf_gen_requestid();

    /**
     * 设置PUSH类消息的响应callback
     * @param cb 
     */
    virtual void taf_set_push_callback(const ServantProxyCallbackPtr& cb);

    /**
     * TAF协议同步方法调用
     */
    virtual void taf_invoke(char cPacketType, 
                            const string& sFuncName, 
                            const vector<char> &buf, 
                            const map<string, string>& context, 
                            const map<string, string>& status, 
                            ResponsePacket& rep);

    /**
     * TAF协议异步方法调用
     */
    virtual void taf_invoke_async(char cPacketType,
                                  const string& sFuncName, 
                                  const vector<char> &buf, 
                                  const map<string, string>& context, 
                                  const map<string, string>& status, 
                                  const ServantProxyCallbackPtr& callback);

    /**
     * 普通协议同步远程调用
     */
    virtual void rpc_call(uint32_t requestId, const string& sFuncName, 
                          const char* buff, uint32_t len, ResponsePacket& rsp);

    /**
     * 普通协议异步调用
     */
    virtual void rpc_call_async(uint32_t requestId, const string& sFuncName, 
                                const char* buff, uint32_t len, 
                                const ServantProxyCallbackPtr& callback);

private:
    /**
     * 远程方法调用
     * @param req
     * @return int
     */
    virtual int invoke(ReqMessagePtr& req);

    /**
     * 远程方法调用返回
     * @param req
     * @return int
     */
    virtual int finished(ReqMessagePtr& req);

private:
    friend class ObjectProxy;
    friend class AdapterProxy;

    /**
     * 所属的ObjectPrx
     */
    ObjectProxy* _objectProxy;

    /**
     * 业务设置的路由器
     */
    ServantProxyRouterPtr _router;

    /**
     * 超时(毫秒)
     */
    int _timeout;
};
//////////////////////////////////////////////////////////////////////////
}
#include "servant/ObjectProxy.h"
//////////////////////////////////////////////////////////////////////////
#endif
