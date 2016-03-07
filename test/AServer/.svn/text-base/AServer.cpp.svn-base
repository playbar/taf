#include "AServer.h"
#include "AImp.h"

using namespace std;

AServer g_app;

/////////////////////////////////////////////////////////////////
void
AServer::initialize()
{
	//initialize application here:
	//...

	addServant<AImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".AObj");

    TafRollLogger::getInstance()->logger()->setLogLevel(TC_RollLogger::INFO_LOG);
//	bindString("tcp -h 127.0.0.1 -t 34543 -p 33333");

}
/////////////////////////////////////////////////////////////////
void
AServer::destroyApp()
{
	//destroy application here:
	//...

    LOG->debug() << "destroyApp" << endl;
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
	try
	{
		g_app.main(argc, argv);
		g_app.waitForShutdown();
        LOG->debug() << "terminate" << endl;
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
