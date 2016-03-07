#ifndef _AServer_H_
#define _AServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace taf;

/**
 * 处理类, 每个处理线程一个对象
 * 继承TC_EpollServer::Handle并实现相应的方法
 */
class StringHandle : public TC_EpollServer::Handle, public BaseNotify
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
		//注册处理函数：
		TAF_ADD_ADMIN_CMD_NORMAL("D", StringHandle::procDLOG);
    }

    /**
     * 具体处理逻辑
     * @param stRecvData
     */
    virtual void handle(const TC_EpollServer::tagRecvData &stRecvData)
    {
		TC_LockT<TC_ThreadRecMutex> lock(*this);

        try
        {
            sendResponse(stRecvData.uid, "ok\r\n", stRecvData.ip, stRecvData.port);
        }
        catch(exception &ex)
        {
            close(stRecvData.uid);
            cout << ex.what() << endl;
        }
    }

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
protected:
	bool procDLOG(const string& command, const string& params, string& result)
	{
		cout << command << endl;
		return true;
	}
};


/**
 *
 **/
class AServer : public Application
{
public:
	/**
	 *
	 **/
	virtual ~AServer() {};

	/**
	 *
	 **/
	virtual void initialize();

	/**
	 *
	 **/
	virtual void destroyApp();


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
        lsPtr->setProtocol(&StringHandle::parse);
		//设置处理器
        lsPtr->setHandle<StringHandle>();

		//完成绑定
        getEpollServer()->bind(lsPtr);
    }

};

extern AServer g_app;

////////////////////////////////////////////
#endif
