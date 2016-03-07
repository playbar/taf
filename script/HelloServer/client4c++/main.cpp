#include "servant/Communicator.h"
#include "util/tc_common.h"
#include "util/tc_monitor.h"
#include "util/tc_clientsocket.h"
#include "util/tc_http.h"
#include "log/taf_logger.h"

#include <sys/un.h>
#include <iostream>
#include "Hello.h"

using namespace std;
using namespace taf;
using namespace Test;

TC_RollLogger logger;

CommunicatorPtr _comm = new Communicator();
HelloPrx pPrx;

void syncCallFast()
{
//		char *c = new char[1024*1024*1];

//		string s;
//		s.assign(c, 1024*1024*1);
		int i = 10000000;
	while(i)
	{
    try
	{
//			LOG->debug() << "begin" << endl;

    	//发起远程调用
		string s = "abc";
    	string r;
//    	logger.debug() << "testHelloFast send sync:" << s << endl;
    	int ret = pPrx->testHelloFast(s, r);
    //	assert(ret == 0);
    //	assert(s == r);
//   	logger.debug() << "testHelloFast recv sync:" << r << endl;
//			LOG->debug() << "end" << endl;
//			sleep(2);
	}
    catch(exception &ex)
	{
		logger.debug() << ex.what() << endl;
	}
	i--;
	}
}

void syncCallSlow(HelloPrx pPrx, int i)
{
    try
	{
    	//发起远程调用
    	string s = TC_Common::tostr(i);
    	string r;
    	logger.debug() << "testHelloSlow send sync:" << s << endl;
    	int ret = pPrx->testHelloSlow(s, r);
    //	assert(ret == 0);
    //	assert(s == r);
    	logger.debug() << "testHelloSlow recv sync:" << r << endl;
	}
    catch(exception &ex)
	{
		logger.debug() << ex.what() << endl;
	}
}

TC_ThreadLock l;

struct HelloCallback : public HelloPrxCallback
{
	//回调函数
	virtual void callback_testHello(int ret, const string &r)
	{
		assert(ret == 0);
		assert(r == "hello world");
		logger.debug() << "callback:" << r << endl;

        TC_ThreadLock::Lock lock(l);
		l.notify();
	}

	virtual void callback_testHello_exception(taf::Int32 ret)
	{
		assert(ret == 0);
		logger.debug() << "callback exception:" << ret << endl;

        TC_ThreadLock::Lock lock(l);
		l.notify();
	}
};

void asyncCall(HelloPrx pPrx)
{
	HelloPrxCallbackPtr p = new HelloCallback;
	//发起远程调用
	string s = "hello world";
	string r;
	logger.debug() << "send async:" << s << endl;

	TC_ThreadLock::Lock lock(l);

	pPrx->async_testHello(p, s);

	l.wait();

    logger.debug() << "async call ok" << endl;
}

void stringCall()
{
    string s = "hello world\r\n";
    TC_TCPClient t;
    t.init("127.0.0.1", 8001, 3000);
    string r;
    logger.debug() << "string call:" << s;
    int ret = t.send(s.c_str(), s.length());
    logger.debug() << ret << endl;
    ret = t.recvBySep(r, "\r\n");
    assert(ret == 0);
    logger.debug() << ret << ":" << r << endl;
}

void httpGetCall()
{
    TC_HttpRequest req;
    req.setContentType("text/plain");
    req.setGetRequest("http://172.25.38.67:8000");

    logger.debug() << "http call:" << req.encode() << endl;
    TC_HttpResponse res;
    int ret = req.doRequest(res, 3000);
    assert(ret == 0);
    logger.debug() << "http recv:" << ret << endl;
    logger.debug() << res.getContent() << endl;
}

void httpPostCall()
{
    TC_HttpRequest req;
    req.setContentType("text/plain");
    req.setPostRequest("http://172.25.38.67:8000", "name=value\r\n");

    logger.debug() << "http call:" << req.encode() << endl;
    TC_HttpResponse res;
    int ret = req.doRequest(res, 3000);
    assert(ret == 0);
    logger.debug() << "http recv:" << ret << endl;
    logger.debug() << res.getContent() << endl;
}

int main(int argc, char *argv[])
{
    try
	{
//			fork();fork();fork();
//			fork();
		//生成客户端代理
//		pPrx = _comm->stringToProxy<HelloPrx>("Test.HelloServer.HelloObj@tcp -h 127.0.0.1 -p 45454");
		pPrx = _comm->stringToProxy<HelloPrx>("Test.HelloServer.HelloObj@tcp -h 127.0.0.1 -p 45455");
//		HelloPrx pPrx = _comm->stringToProxy<HelloPrx>("Test.HelloServer.HelloObj@tcp -h 127.0.0.1 -p 44444");

//		int i = 1000000;

		TC_ThreadPool tpool;
		tpool.init(10);
		tpool.start();
		
		TC_Functor<void> cmd(syncCallFast);
   		TC_Functor<void>::wrapper_type fwrapper(cmd);
   		tpool.exec(fwrapper);

		tpool.waitForAllDone();

		return 0;

		int i = 5;
		while(i)
		{
            if(argc > 1)
            {
                syncCallFast();
            }
            else
            {
                syncCallSlow(pPrx, i);
            }
			i--;
		}

		//异步调用
//		asyncCall(pPrx);
        return 0;

        /*
        //字符串调用
        stringCall();

        httpGetCall();
        httpPostCall();
        */
	}
	catch(exception &ex)
	{
        logger.debug() << ex.what() << endl;
	}

	return 0;
}



