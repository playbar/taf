#ifndef __TC_HTTP_ASYNC_H_
#define __TC_HTTP_ASYNC_H_

#include "util/tc_thread_pool.h"
#include "util/tc_http.h"
#include "util/tc_autoptr.h"
#include "util/tc_socket.h"
//#include "util/tc_timeoutQueue.h"

namespace taf
{

/////////////////////////////////////////////////
// 说明: http异步调用类
//       http同步调用使用TC_HttpRequest::doRequest就可以了
// Author : j@syswin.com              
/////////////////////////////////////////////////

/**
 * 异步HTTP请求 
 * 使用方式示例如下:
    //实现异步回调对象
    //异步对象回调执行的时候是在TC_HttpAsync中线程执行的
    //用智能指针new出来, 不用管生命周期
    class AsyncHttpCallback : public TC_HttpAsync::RequestCallback
    {
    public:
        AsyncHttpCallback(const string &sUrl) : _sUrl(sUrl)
        {
        }
        virtual void onException(const string &ex) 
        {
            cout << "onException:" << _sUrl << ":" << ex << endl;
        }
        //请求回来的时候onResponse被调用
        //bClose表示服务端关闭了连接, 从而认为收到了一个完整的http响应
        virtual void onResponse(bool bClose, TC_HttpResponse &stHttpResponse) 
        {
            cout << "onResponse:" << _sUrl << ":" << TC_Common::tostr(stHttpResponse.getHeaders()) << endl;
        }
        virtual void onTimeout() 
        {
            cout << "onTimeout:" << _sUrl << endl;
        }
        //连接被关闭时调用
        virtual void onClose()
        {
            cout << "onClose:" << _sUrl << endl;
        }
    protected:
        string _sUrl;
    };

    //封装一个函数, 根据实际情况处理
    int addAsyncRequest(TC_HttpAsync &ast, const string &sUrl)
    {
        TC_HttpRequest stHttpReq;
        stHttpReq.setGetRequest(sUrl);
 
        //new出来一个异步回调对象
        TC_HttpAsync::RequestCallbackPtr p = new AsyncHttpCallback(sUrl);

        return ast.doAsyncRequest(stHttpReq, p);    
    }
 
    //具体使用的示例代码如下:
    TC_HttpAsync ast;
    ast.setTimeout(1000);  //设置异步请求超时时间
    ast.start();
 
    //真正的代码需要判断返回值,返回值=0才表示请求已经发送出去了
    int ret = addAsyncRequest(ast, "www.baidu.com");    

    addAsyncRequest(ast, "www.qq.com");
    addAsyncRequest(ast, "www.google.com");
    addAsyncRequest(ast, "http://news.qq.com/a/20100108/002269.htm");
    addAsyncRequest(ast, "http://news.qq.com/zt/2010/mtjunshou/");
    addAsyncRequest(ast, "http://news.qq.com/a/20100108/000884.htm");
    addAsyncRequest(ast, "http://tech.qq.com/zt/2009/renovate/index.htm");

    ast.waitForAllDone();
    ast.terminate();
 */

/**
* 线程异常
*/
struct TC_HttpAsync_Exception : public TC_Exception
{
    TC_HttpAsync_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_HttpAsync_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_HttpAsync_Exception() throw(){};
};

/**
* 异步线程处理类
*/
class TC_HttpAsync : public TC_Thread, public TC_ThreadLock
{
public:
    /**
     * 异步请求回调对象
     */
    class RequestCallback : public TC_HandleBase
    {
    public:
        /**
         * 完整的响应回来了
         * @param bClose: 因为远程服务器关闭连接认为http完整了
         * @param stHttpResponse: http响应包
         */
        virtual void onResponse(bool bClose, TC_HttpResponse &stHttpResponse) = 0;

        /**
         * 每次收到数据且http头收全了都会调用
         * (stHttpResponse的数据可能不是完全的http响应数据, 只有部分body数据)
         * @param stHttpResponse
         * @return true:继续收取数据, false:不收取数据了
         */
        virtual bool onReceive(TC_HttpResponse &stHttpResponse) { return true;};

        /**
         * 异常
         * @param ex: 异常原因
         */
        virtual void onException(const string &ex) = 0;

        /**
         * 超时没有响应
         */
        virtual void onTimeout() = 0;

        /**
         * 连接被关闭
         */
        virtual void onClose() = 0;
    };

    typedef TC_AutoPtr<RequestCallback> RequestCallbackPtr;

protected:
    /**
     * 异步http请求(短连接)
     */
    class AsyncRequest : public TC_HandleBase
    {
    public:
        /**
         * 构造
         * @param stHttpRequest
         * @param callbackPtr
         */
        AsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr);

        /**
         * 析构
         */
        ~AsyncRequest();

        /**
         * 句柄
         * 
         * @return int
         */
        int getfd() { return _fd.getfd(); }

        /**
         * 发起建立连接
         * 
         * @return int
         */
        int doConnect();

        /**
         * 发起建立到addr的连接,不用DNS解析
         * @param addr 可用于直接连接透明代理或者连接通过DNS解析后的地址
         * @return int
         */
        int doConnect(struct sockaddr* addr);

        /**
         * 发生异常
         */
        void doException();

        /**
         * 发送请求
         */
        void doRequest();

        /**
         * 接收响应
         */
        void doReceive();

        /**
         * 关闭连接
         */
        void doClose();

        /**
         * 超时
         */
        void timeout();

        /**
         * 唯一ID
         * @param uniqId
         */
        void setUniqId(uint32_t uniqId)    { _iUniqId = uniqId;}

        /**
         * 获取唯一ID
         * 
         * @return uint32_t
         */
        uint32_t getUniqId() const         { return _iUniqId; }
           
        /**
         * 设置处理请求的http异步线程
         * 
         * @param pHttpAsync 
         */
        void setHttpAsync(TC_HttpAsync *pHttpAsync) { _pHttpAsync = pHttpAsync; }

        /**
         * 设置发网络请求时绑定的ip地址
         * @param addr
         */
		void setBindAddr(const struct sockaddr* addr);

    protected:
        /**
         * 接收请求
         * @param buf
         * @param len
         * @param flag
         * 
         * @return int
         */
        int recv(void* buf, uint32_t len, uint32_t flag);

        /**
         * 发送请求
         * @param buf
         * @param len
         * @param flag
         * 
         * @return int
         */
        int send(const void* buf, uint32_t len, uint32_t flag);

    protected:
        TC_HttpAsync               *_pHttpAsync;
        TC_HttpResponse             _stHttpResp;
        TC_Socket                   _fd;
        string                      _sHost;
        uint32_t                    _iPort;
        uint32_t                    _iUniqId;
        string                      _sReq;
        string                      _sRsp;
        RequestCallbackPtr          _callbackPtr;
		bool						_bindAddrSet;
		struct sockaddr 			_bindAddr;
    };

    typedef TC_AutoPtr<AsyncRequest> AsyncRequestPtr;

public:

    typedef TC_TimeoutQueue<AsyncRequestPtr> http_queue_type;

    /**
     * 构造函数
     */
    TC_HttpAsync();

    /**
     * 析构函数
     */
    ~TC_HttpAsync();

    /**
     * 异步发起请求,
     * @param stHttpRequest
     * @param httpCallbackPtr
     * @param bUseProxy,是否使用代理方式连接
     * @param addr, bUseProxy为false 直接连接指定的地址 
     * @return int, <0:发起连接失败, 可以通过strerror(返回值)
     *             =0:成功
     */
    int doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, bool bUseProxy=false, struct sockaddr* addr=NULL);

    /**
     * 设置proxy地址
     * 
     */
    int setProxyAddr(const char* Host,uint16_t Port);

    /**
     * 设置代理的地址(不通过域名解析发送,直接发送到代理服务器的ip地址)
     * @param sProxyAddr 格式 192.168.1.2:2345 或者 sslproxy.qq.com:2345
	 */
    int setProxyAddr(const char* sProxyAddr);

    /**
     * 设置绑定的地址
	 * @param sProxyAddr 格式 192.168.1.2
	 */
	int setBindAddr(const char* sBindAddr);

    /**
     * 设置绑定的地址
	 * @param addr 直接用 addr 赋值
	 */
	void setProxyAddr(const struct sockaddr* addr);

    /**
     * 启动异步处理
     * 参数已经无效(网络层有且只有一个线程)
     * @param num, 异步处理的线程数
     */
    void start(int iThreadNum = 1);

    /**
     * 设置超时(所有请求都只能用一种超时时间)
     * @param timeout: 毫秒, 但是具体的超时精度只能在s左右
     */
    void setTimeout(int millsecond) { _data->setTimeout(millsecond); }

    /**
     * 等待请求全部结束(等待毫秒精度在100ms左右)
     * @param millsecond, 毫秒 -1表示永远等待
     */
    void waitForAllDone(int millsecond = -1);

    /**
     * 结束线程
     */
    void terminate();

protected:

    typedef TC_Functor<void, TL::TLMaker<AsyncRequestPtr, int>::Result> async_process_type;

    /**
     * 超时处理
     * @param ptr
     */
    static void timeout(AsyncRequestPtr& ptr);

    /**
     * 具体网络处理
     */
    static void process(AsyncRequestPtr &p, int events);

    /**
     * 具体的网络处理逻辑
     */
    void run() ;

    /**
     * 删除异步请求对象
     */
    void erase(uint32_t uniqId); 

    friend class AsyncRequest;

protected:
    TC_ThreadPool               _tpool;

//    vector<TC_ThreadPool*>      _npool;

    http_queue_type             *_data;
    
    TC_Epoller                  _epoller;

    bool                        _terminate;

    struct sockaddr             _proxyAddr;

    struct sockaddr             _bindAddr;

	bool			            _bindAddrSet;
};

}
#endif

