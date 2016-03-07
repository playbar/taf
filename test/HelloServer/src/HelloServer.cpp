#include "HelloServer.h"
#include "HelloImp.h"

using namespace std;

string HelloServer::CCSERVER_OBJ = "CC.CCServer.Object@tcp -h 172.25.38.142 -p 40195 -t 10000";

/////////////////////////////////////////////////////////////////
void
HelloServer::initialize()
{
	addServant<HelloImp>("Test.HelloServer.HelloObj");

    TC_EpollServer::protocol_functor func = AppProtocol::parseStream<0, uint32_t, true>;

    Application::getEpollServer()->getBindAdapter("HelloAdapter")->setProtocol(func);

    ServantPrx proxy = NULL;

    Application::getCommunicator()->stringToProxy(HelloServer::CCSERVER_OBJ, proxy);

    ProxyProtocol prot;

    prot.requestFunc = ProxyProtocol::streamRequest;

    prot.responseFunc = ProxyProtocol::streamResponse<0, uint32_t, true, 9, uint32_t, true>;

    proxy->taf_set_protocol(prot);

	//从配置中心获取配置文件
    //addConfig("HelloServer.conf");
	//注册处理函数：
    //TAF_ADD_ADMIN_CMD_NORMAL("DISABLEDLOG", HelloServer::procDLOG);
	//加载http服务
	///bind("tcp -h 127.0.0.1 -p 9000 -t 60000");
}

/////////////////////////////////////////////////////////////////
void HelloServer::destroyApp()
{
	//destroy application here:
	//...
}

/////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	try
	{
        HelloServer server;

		server.main(argc, argv);

		server.waitForShutdown();
	}
	catch (std::exception& e)
	{
		cerr << "std::exception:" << e.what() << std::endl;
	}
	catch (...)
	{
		cerr << "unknown exception." << std::endl;
	}
	return -1;
}
/////////////////////////////////////////////////////////////////
