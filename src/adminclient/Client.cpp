#include "Client.h"
#include "servant/CommunicatorFactory.h"
#include "util/tc_clientsocket.h"
#include "log/taf_logger.h"
#include "servant/AdminF.h"

int debug = 0;

//初始化管理端口
int Client::init(TC_Config & conf)
{
    try
    {
        //_comm = new Communicator(conf);
        _comm = CommunicatorFactory::getInstance()->getCommunicator(conf);

        _adminPtr = _comm->stringToProxy<AdminRegPrx>("taf.tafregistry.AdminRegObj");

        _nodePtr  = _comm->stringToProxy<NodePrx>(conf.get("/taf/application/server<node>", ""));

        TafRollLogger::getInstance()->setLogInfo("taf", "tafadmin", conf.get("/taf/application/server<logpath>", "./"));
    }
    catch(TafException & ex)
    {
        cout<<"init error:"<<ex.what()<<endl;
        exit(-1);
    }

    return 0;
}


int Client::doNotify(const string & ipport, const string & command, string & result)
{
    try
    {
        string::size_type ipos = ipport.find(":");
        if(ipos == string::npos)
        {
            result = "param ipport should be Ip:Port eg: 127.0.0.1:12345";
            return -1;
        }

        taf::Communicator comm;
        AdminFPrx pAdminPrx;    //服务管理代理
        pAdminPrx = comm.stringToProxy<AdminFPrx>("AdminObj@tcp -h " + ipport.substr(0, ipos) + 
                " -p " + ipport.substr(ipos+1) + " -t 10000");
        result = pAdminPrx->notify(command);
    }
    catch (exception& e)
    {
        result = "error " + string(e.what());
        return -1;
    }
    return 0;
}


void Client::usage(int argc, char *argv[])
{
    cout << "Usage:" << argv[0] << " --config=ConfigFile [--debug]" << endl;
    cout << "      " << argv[0] << " --ipport=Ip:Port --command=NotiryCommand" << endl;
    exit(0);
}

int Client::run()
{
    ParserPtr p = Parser::createParser(_adminPtr, _nodePtr);

    p->showBanner();

    p->parse(stdin, debug);

    return 0;
}

int main(int argc, char *argv[])
{
    try
    {
        Client clt;
        if(argc < 2)
        {
            clt.usage(argc, argv);
        }

        TC_Option option;
        option.decode(argc, argv);

        //直接notify控制端口
        string ipport= option.getValue("ipport");
        if(ipport != "")
        {
            string command = option.getValue("command");
            if(command == "")
            {
                clt.usage(argc, argv);
            }
            else
            {
                string result;
                clt.doNotify(ipport, command, result);
                cout << result << endl;
            }
            exit(0);
        }

        //控制台
        string config = option.getValue("config");
        
        if(config == "")
        {
            clt.usage(argc, argv);
            exit(0);
        }
        //open yydebug
        if(option.hasParam("debug"))
        {
            debug = 1;
        }

        TC_Config   _conf;
        _conf.parseFile(config);

        clt.init(_conf);

        clt.run();

    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }

    return 0;
}


