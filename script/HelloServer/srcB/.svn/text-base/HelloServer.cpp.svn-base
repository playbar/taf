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

//	addConfig("HelloServer.conf");

//	setThreadContext(true);
//    setThreadStackSize(1024*32);

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
     	TafRollLogger::getInstance()->sync(true);
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
