#include "HelloServer.h"
#include "HelloImp.h"

using namespace std;

HelloServer g_app;

/////////////////////////////////////////////////////////////////
void
HelloServer::initialize()
{
	//initialize application here:
	//...

	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".HelloObj");

	//从配置中心获取配置文件
//	addConfig("HelloServer.conf");

	//注册处理函数：
    TAF_ADD_ADMIN_CMD_NORMAL("DISABLEDLOG", HelloServer::procDLOG);

	//加载http服务
	///bind("tcp -h 127.0.0.1 -p 9000 -t 60000");
}

/////////////////////////////////////////////////////////////////
void
HelloServer::destroyApp()
{
	//destroy application here:
	//...
}

/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
	try
	{
		g_app.main(argc, argv);
		g_app.waitForShutdown();
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
