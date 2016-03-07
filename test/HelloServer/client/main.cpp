#include "servant/Communicator.h"
#include "util/tc_common.h"
#include "util/tc_monitor.h"
#include <sys/un.h>
#include <iostream>
#include "Hello.h"
#include "log/taf_logger.h"

using namespace std;
using namespace taf;
using namespace Test;

CommunicatorPtr _comm = new Communicator();

static int _grid = 0;
static string _locator = 
//"Test.HelloServer.HelloObj";
"Test.HelloServer.HelloObj@tcp -h 172.25.38.142 -p 45460";
//"Test.HelloServer.HelloObj@tcp -h 172.25.38.67 -p 45460 -g 0:tcp -h 172.25.38.67 -p 45461 -g 0";
//"Test.HelloTransfer.HelloObj@tcp -h 172.25.38.67 -p 45457 -g 0:tcp -h 172.25.38.67 -p 45458 -g 0:tcp -h 172.25.38.67 -p 45459 -g 0";

void syncCall(HelloPrx pPrx, int c)
{
	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		string r;

		_grid = (i%13 == 0 ? 1 : 0);

		ostringstream os; os << "[hello-grid:" << _grid << ":" << i << "]";

		cout << "send:" << os.str() << "|";

		try
		{
			pPrx->testHello(os.str(), r);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}

		cout << "recv:" << r << endl;
	}
}

TC_ThreadLock l;

struct HelloCallback : public HelloPrxCallback
{
	//回调函数
	virtual void callback_testHello(int ret, const string &r)
	{
		assert(ret == 0);

		cout << "callback:" << r << endl;
	}

	virtual void callback_testHello_exception(taf::Int32 ret)
	{
		cout << "callback exception:" << ret << endl;
	}
};

void asyncCall(HelloPrx pPrx, int c)
{
	//发起远程调用
	for (int i = 1; i < c; ++i)
	{
		HelloPrxCallbackPtr p = new HelloCallback;

		_grid = 0;//(i%13 == 0 ? 1 : 0);

		ostringstream os; os << "{hello:" << i << ",grid:" << _grid << "}";

		try
		{
			pPrx->async_testHello(p, os.str());
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}

		if (i % 3000 == 0) 
		{
			sleep(1);
			pid_t pid = getpid();
			timeval tv;
			gettimeofday(&tv, NULL);
			cout << "[pid:" << pid << "][count:" << i << "][now:" << tv.tv_sec << ":" << tv.tv_usec << "]" << endl;
		}
	}
}

struct TestRouter : public taf::ServantProxyRouter
{
	int getGridByKey(const string& key)
	{
		return _grid;
	}
};

int main(int argc, char *argv[])
{
    try
	{
        if (argc < 5)
        {
            cout << "Usage:" << argv[0] << " <config file> <count> <sync:0|async:1> <fork num>" << endl;

            return 0;
        }

		int f = TC_Common::strto<int>(argv[4]);

		for (int i = 0; i < f; ++i)
		{
			fork();
		}

		TafRollLogger::getInstance()->logger()->setLogLevel(4);

        TC_Config conf;

        conf.parseFile(argv[1]);

		_comm->setProperty(conf);

		HelloPrx pPrx = _comm->stringToProxy<HelloPrx>(_locator);

		/*
		ServantProxyRouterPtr router = new TestRouter();

		pPrx->taf_set_router(router);
		*/

		int c = TC_Common::strto<int>(argv[2]);

		timeval tv1, tv2;
		gettimeofday(&tv1, NULL);

		int t = TC_Common::strto<int>(argv[3]);

		if (t == 0) 
		{
		    syncCall(pPrx, c);
		}
		else if (t == 1) 
		{
			asyncCall(pPrx, c);
		}

		pid_t pid = getpid();
		gettimeofday(&tv2, NULL);

		cout << "(pid:" << pid << ")" 
			 << "(" << (t == 0 ? "sync" : "async") << ")"
			 << "(count:" << c << ")"
			 << "(use ms:" << (tv2.tv_sec - tv1.tv_sec)*1000 + (tv2.tv_usec - tv1.tv_usec)/1000 << ")"
			 << endl;

		///sleep(3);
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}
	cout << "main return." << endl;

	return 0;
}


/*
class PayServant
{
    virtual int doPayCoin(const UserInfo& tUserInfo, 
                          bool mode, string& operId, 
                          JceCurrentPtr current) = 0;
};

class PayProxy
{
    int doPayCoin(const UserInfo& tUserInfo, 
                  bool mode, string& operId, 
                  const map<string, string>& context);

    void async_doPayCoin(PayProxyCallbackPtr callback, 
                         const UserInfo& tUserInfo, 
                         bool mode, string& operId, 
                         const map<string, string>& context);
};
*/
