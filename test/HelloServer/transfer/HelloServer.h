#ifndef _HelloServer_H_
#define _HelloServer_H_

#include <iostream>
#include "util/tc_http.h"
#include "util/tc_cgi.h"
#include "servant/Application.h"

using namespace taf;

struct HttpProtocol
{
    static int parse(string &in, string &out)
    {
		//TAF的util里面提供了http协议解析类
        taf::TC_HttpRequest request;
        if(request.decode(in))
        {
            out = in;
            in  = "";
            return 1;	//返回1表示收到的包已经完全
        }

        return 0;		//返回0表示收到的包不完全
//		return -1;	//返回-1表示收到包协议错误，框架会自动关闭当前连接
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
	/*
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
            os << cgi.isOverUploadFiles();

			//组装响应包
            TC_HttpResponse response;
            response.setResponse(200, "OK", os.str());
            response.setConnection("close");
            string buffer = response.encode();

            sendRespone(stRecvData.uid, buffer, stRecvData.ip, stRecvData.port);
        }
        catch(exception &ex)
        {
            close(stRecvData.uid);
            cout << ex.what() << endl;
        }
    }

	//过载保护的时候会调用该函数
    virtual void handleOverload(const TC_EpollServer::tagRecvData &stRecvData)
    {
        close(stRecvData.uid);
    }
	*/
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
	/*
    void bind(const string &str)
    {
        TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(getEpollServer().get());

		//设置ip,port，这里str的格式是:tcp -h 127.0.0.1 -p 80 -t 60000
        lsPtr->setEndpoint(str);
		//设置处理线程数
        lsPtr->setHandleNum(4);
		//设置协议解析器
        lsPtr->setProtocol(&HttpProtocol::parse);
		//设置处理器
        lsPtr->setHandle<HttpHandle>();

		//完成绑定
        getEpollServer()->bind(lsPtr);
    }
	*/

	bool procDLOG(const string& command, const string& params, string& result)
	{
		TafTimeLogger::getInstance()->enableLocal(params, false);
		return false;
	}
};

extern HelloServer g_app;

////////////////////////////////////////////
#endif
