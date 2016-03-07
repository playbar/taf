#ifndef _HelloServer_H_
#define _HelloServer_H_

#include <iostream>
#include "util/tc_http.h"
#include "util/tc_cgi.h"
#include "util/tc_parsepara.h"
#include "servant/Application.h"

using namespace taf;

/////////////////////////////////////////////////////////////////////////////
// 以下是http协议的例子
struct HttpProtocol
{
    static int parse(string &in, string &out)
    {
        cout << in;
        try
        {
    		//TAF的util里面提供了http协议解析类
            taf::TC_HttpRequest request;
            if(request.decode(in))
            {
                out = in;
                in  = "";
                return TC_EpollServer::PACKET_FULL;	//表示收到的包已经完全
            }
        }
        catch(exception &ex)
        {
            cout << ex.what() << endl;
            return TC_EpollServer::PACKET_ERR;
        }

        return TC_EpollServer::PACKET_LESS;		//表示收到的包不完全
//		return TC_EpollServer::PACKET_ERR;	//表示收到包协议错误，框架会自动关闭当前连接
    }
};

/**
 * 处理类, 每个处理线程一个对象
 * 继承TC_EpollServer::Handle并实现相应的方法
 */
class HttpHandle : public TC_EpollServer::Handle
{
public:
    /**
     * 构造函数, 线程启动时会构造出对象
     */
    HttpHandle()
    {}

	//初始化，构造出来后，会调用该函数
    virtual void initialize()
    { 
    }

    /**
     * 具体处理逻辑
     * @param stRecvData
     */
    virtual void handle(const TC_EpollServer::tagRecvData &stRecvData)
    {
        try
        {
            TC_HttpRequest request;
            request.decode(stRecvData.buffer);
			
            string sFileName = "/home/jarod/taf/test/util/" + TC_Common::now2str() + ".tmp";

            TC_Cgi cgi;

            //设置上传文件路径和最大文件个数
            cgi.setUpload(sFileName, 10);
            cgi.parseCgi(request);

            ostringstream os;
            os << TC_Common::tostr(cgi.getUploadFilesMap());

            os << TC_Common::tostr(cgi.getParamMap());
            os << TC_Common::tostr(cgi.isOverUploadFiles());

			//组装响应包
            TC_HttpResponse response;
            response.setResponse(200, "OK", os.str());
            response.setConnection("close");
            string buffer = response.encode();

            cout << buffer << endl;
            sendResponse(stRecvData.uid, buffer, stRecvData.ip, stRecvData.port);
        }
        catch(exception &ex)
        {
            close(stRecvData.uid);
            cout << ex.what() << endl;
        }
    }

	//过载保护的时候会调用该函数
    virtual void handleTimeout(const TC_EpollServer::tagRecvData &stRecvData)
    {
        close(stRecvData.uid);
    }
};

//////////////////////////////////////////////////////////////////////////////////
// 以下是字符串协议的列子
//////////////////////////////////////////////////////////////////////////////////
// 以下是字符串协议的列子
struct StringProtocol
{
    static int parse(string &in, string &out)
    {
        cout << in;
        string::size_type pos = in.find("\r\n");

        if(pos != string::npos)
        {
            out = in.substr(0, pos);
            in  = in.substr(pos+2);

            return TC_EpollServer::PACKET_FULL;   //返回1表示收到的包已经完全
        }

        return TC_EpollServer::PACKET_LESS;		//返回0表示收到的包不完全
//		return TC_EpollServer::PACKET_ERR;	    //返回-1表示收到包协议错误，框架会自动关闭当前连接
    }
};


/**
 * 处理类, 每个处理线程一个对象
 * 继承TC_EpollServer::Handle并实现相应的方法
 */
class StringHandle : public TC_EpollServer::Handle
{
public:
    /**
     * 构造函数, 线程启动时会构造出对象
     */
    StringHandle()
    {}

	//初始化，构造出来后，会调用该函数
    virtual void initialize()
    { 
    }

    /**
     * 具体处理逻辑
     * @param stRecvData
     */
    virtual void handle(const TC_EpollServer::tagRecvData &stRecvData)
    {
        try
        {
            //taf提供了name=value的解析类
            taf::TC_Parsepara para(stRecvData.buffer);

            cout << TC_Common::tostr(para.toMap()) << endl;
			
            sendResponse(stRecvData.uid, "ok\r\n", stRecvData.ip, stRecvData.port);
        }
        catch(exception &ex)
        {
            close(stRecvData.uid);
            cout << ex.what() << endl;
        }
    }

	//过载保护的时候会调用该函数
    virtual void handleTimeout(const TC_EpollServer::tagRecvData &stRecvData)
    {
        close(stRecvData.uid);
    }
};

/**
 *
 **/
class HelloServer : public Application
{
public:
	/**
	 *
	 **/
	virtual ~HelloServer() {};

	/**
	 *
	 **/
	virtual void initialize();

	/**
	 *
	 **/
	virtual void destroyApp();

protected:

	//定义一个函数，用来完成初始化，绑定端口之类
    void bindHttp(const string &str)
    {
        TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(getEpollServer().get());

        lsPtr->setName("HttpAdapter");
		//设置ip,port，这里str的格式是:tcp -h 127.0.0.1 -p 80 -t 60000
        lsPtr->setEndpoint(str);
        //设置最大连接数
        lsPtr->setMaxConns(10240);
        //设置队列长度(默认是10240个元素), 如果超过了限制, 则直接关闭后续的请求连接
        lsPtr->setQueueCapacity(1000);
        //设置队列数据的超时时间, 如果数据出队列时超时, 则handleTimeout被调用, 默认是关闭连接的, 业务可以继承handle时重载
        lsPtr->setQueueTimeout(10000);
		//设置处理线程数
        lsPtr->setHandleNum(2);
		//设置协议解析器
        lsPtr->setProtocol(&HttpProtocol::parse);
		//设置处理器
        lsPtr->setHandle<HttpHandle>();

		//完成绑定
        getEpollServer()->bind(lsPtr);

    }

	//定义一个函数，用来完成初始化，绑定端口之类
    void bindString(const string &str)
    {
        TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(getEpollServer().get());

        lsPtr->setName("StringAdapter");

		//设置ip,port，这里str的格式是:tcp -h 127.0.0.1 -p 80 -t 60000
        lsPtr->setEndpoint(str);
		//设置处理线程数
        lsPtr->setHandleNum(2);
		//设置协议解析器
        lsPtr->setProtocol(&StringProtocol::parse);
		//设置处理器
        lsPtr->setHandle<StringHandle>();

		//完成绑定
        getEpollServer()->bind(lsPtr);
    }
protected:
	bool procDLOG(const string& command, const string& params, string& result)
	{
		TafTimeLogger::getInstance()->enableLocal(params, false);
		return false;
	}
};

extern HelloServer g_app;

////////////////////////////////////////////
#endif
