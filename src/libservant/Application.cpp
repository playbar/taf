#include "util/tc_option.h"
#include "util/tc_common.h"
#include "nodeF/taf_nodeF.h"
#include "servant/Application.h"
#include "servant/AppProtocol.h"
#include "servant/AdminServant.h"
#include "servant/ServantHandle.h"
#include "servant/BaseF.h"
#include "servant/AppCache.h"
#include "servant/NotifyObserver.h"

#include <signal.h>

namespace taf
{

static void sighandler( int sig_no )
{
    Application::terminate();
}

std::string ServerConfig::Application;      //应用名称
std::string ServerConfig::ServerName;       //服务名称,一个服务名称含一个或多个服务标识
std::string ServerConfig::LocalIp;          //本机IP
std::string ServerConfig::BasePath;         //应用程序路径，用于保存远程系统配置的本地目录
std::string ServerConfig::DataPath;         //应用程序路径，用于本地数据
std::string ServerConfig::Local;            //本地套接字
std::string ServerConfig::Node;             //本机node地址
std::string ServerConfig::Log;              //日志中心地址
std::string ServerConfig::Config;           //配置中心地址
std::string ServerConfig::Notify;           //信息通知中心
std::string ServerConfig::LogPath;          //logpath
int         ServerConfig::LogSize;          //log大小(字节)
int         ServerConfig::LogNum;           //log个数()
std::string ServerConfig::ConfigFile;       //框架配置文件路径
int         ServerConfig::ReportFlow;       //是否服务端上报所有接口stat流量 0不上报 1上报 (用于非taf协议服务流量统计)

static string outfill(const string& s, char c = ' ', int n = 29)
{
    return (s + string(abs(n - (int)s.length()), c));
}

#define OUT_LINE        (outfill("", '-', 50))
#define OUT_LINE_LONG   (outfill("", '=', 50))

///////////////////////////////////////////////////////////////////////////////////////////
TC_Config                       Application::_conf;
TC_EpollServerPtr               Application::_epollServer  = NULL;
CommunicatorPtr                 Application::_communicator = NULL;

///////////////////////////////////////////////////////////////////////////////////////////
Application::Application()
{
}

Application::~Application()
{
    terminate();
}

TC_Config& Application::getConfig()
{
    return _conf;
}

TC_EpollServerPtr& Application::getEpollServer()
{
    return _epollServer;
}

CommunicatorPtr& Application::getCommunicator()
{
    return _communicator;
}

void Application::waitForShutdown()
{
    _epollServer->waitForShutdown();

    destroyApp();

    TafRemoteNotify::getInstance()->report("stop", true);
}

void Application::terminate()
{
    if(_epollServer)
    {
        _epollServer->terminate();
    }
}

bool Application::cmdViewStatus(const string& command, const string& params, string& result)
{
    LOG->info() << "Application::cmdViewStatus:" << command << " " << params << endl;

    ostringstream os;

    os << OUT_LINE_LONG << endl;

    os << outfill("[proxy config]:") << endl;

    outClient(os);

    os << OUT_LINE << "\n" << outfill("[server config]:") << endl;

    outServer(os);

    os << OUT_LINE << endl;

    outAllAdapter(os);

    result = os.str();

    return true;
}

bool Application::cmdSetLogLevel(const string& command, const string& params, string& result)
{
    LOG->debug() << "Application::cmdSetLogLevel:" << command << " " << params << endl;

    string level = TC_Common::trim(params);

    int ret = TafRollLogger::getInstance()->logger()->setLogLevel(level);

    if(ret == 0)
    {
        result = "set log level [" + level + "] ok";

        AppCache::getInstance()->set("logLevel",level);
    }
    else
    {
        result = "set log level [" + level + "] error";
    }

    return true;
}

bool Application::cmdLoadConfig(const string& command, const string& params, string& result)
{
    LOG->info() << "Application::cmdLoadConfig:" << command << " " << params << endl;

    string filename = TC_Common::trim(params);

    if (TafRemoteConfig::getInstance()->addConfig(filename, result))
    {
        TafRemoteNotify::getInstance()->report(result);

        return true;
    }

    TafRemoteNotify::getInstance()->report(result);

    return true;
}

bool Application::cmdConnections(const string& command, const string& params, string& result)
{
    LOG->info() << "Application::cmdConnections:" << command << " " << params << endl;

    ostringstream os;

    os << OUT_LINE_LONG << endl;

    map<int, TC_EpollServer::BindAdapterPtr> m = _epollServer->getListenSocketInfo();

    for(map<int, TC_EpollServer::BindAdapterPtr>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
        vector<TC_EpollServer::ConnStatus> v = it->second->getConnStatus();

        os << OUT_LINE << "\n" << outfill("[adater:" + it->second->getName() + "] [connections:" + TC_Common::tostr(v.size())+ "]") << endl;

        os  << outfill("conn-uid", ' ', 15)
            << outfill("ip:port", ' ', 25)
            << outfill("last-time", ' ', 25)
            << outfill("timeout", ' ', 10) << endl;

        for(size_t i = 0; i < v.size(); i++)
        {
            os  << outfill(TC_Common::tostr<uint32_t>(v[i].uid), ' ', 15)
                << outfill(v[i].ip + ":" + TC_Common::tostr(v[i].port), ' ', 25)
                << outfill(TC_Common::tm2str(v[i].iLastRefreshTime,"%Y-%m-%d %H:%M:%S"), ' ', 25)
                << outfill(TC_Common::tostr(v[i].timeout), ' ', 10) << endl;
        }
    }
    os << OUT_LINE_LONG << endl;

    result = os.str();

    return true;
}

bool Application::cmdViewVersion(const string& command, const string& params, string& result)
{
    result = "$" + string(TAF_VERSION) + "$";
    return true;
}

bool Application::cmdLoadProperty(const string& command, const string& params, string& result)
{
    try
    {
        LOG->info() << "Application::cmdLoadProperty:" << command << " " << params << endl;

        //重新解析配置文件
        _conf.parseFile(ServerConfig::ConfigFile);

        string sResult = "";

        //加载通讯器属性
        _communicator->setProperty(_conf);

        _communicator->reloadProperty(sResult);

        //加载远程对象
        ServerConfig::Log = _conf.get("/taf/application/server<log>");

        TafTimeLogger::getInstance()->setLogInfo(_communicator, ServerConfig::Log, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath);

        ServerConfig::Config = _conf.get("/taf/application/server<config>");

        TafRemoteConfig::getInstance()->setConfigInfo(_communicator, ServerConfig::Config, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::BasePath);

        ServerConfig::Notify = _conf.get("/taf/application/server<notify>");

        TafRemoteNotify::getInstance()->setNotifyInfo(_communicator, ServerConfig::Notify, ServerConfig::Application, ServerConfig::ServerName);

        result = "loaded config items:\r\n" + sResult + 
                 "log=" + ServerConfig::Log + "\r\n" + 
                 "config=" + ServerConfig::Config + "\r\n" + 
                 "notify=" + ServerConfig::Notify + "\r\n";
    }
    catch (TC_Config_Exception & ex)
    {
        result = "load config " + ServerConfig::ConfigFile + " error:" + ex.what();
    }
    catch (exception &ex)
    {
        result = ex.what();
    }
    return true;
}
    
bool Application::cmdViewAdminCommands(const string& command, const string& params, string& result)
{
    LOG->info() << "Application::cmdViewAdminCommands:" << command << " " << params << endl;

    result =result +  NotifyObserver::getInstance()->viewRegisterCommand();

    return true;
}

bool Application::cmdSetDyeing(const string& command, const string& params, string& result)
{
    vector<string> vDyeingParams = TC_Common::sepstr<string>(params, " ");

    if(vDyeingParams.size() == 2 || vDyeingParams.size() == 3)
    {
        ServantHelperManager::getInstance()->setDyeing(vDyeingParams[0], vDyeingParams[1], vDyeingParams.size() == 3 ? vDyeingParams[2] : "");
        
        result = "DyeingKey="       + vDyeingParams[0] + "\r\n" +
                 "DyeingServant="   + vDyeingParams[1] + "\r\n" +
                 "DyeingInterface=" + (vDyeingParams.size() == 3 ? vDyeingParams[2] : "") + "\r\n";
    }
    else
    {
        result = "Invalid parameters.Should be: dyeingKey dyeingServant [dyeingInterface]";
    }
    return true;
}

bool Application::cmdCloseCout(const string& command, const string& params, string& result)
{
    LOG->info() << "Application::cmdCloseCout:" << command << " " << params << endl;

    string s = TC_Common::lower(TC_Common::trim(params));

    if(s == "yes")
    {
        AppCache::getInstance()->set("closeCout","1");
    }
    else
    {
        AppCache::getInstance()->set("closeCout","0");
    }

    result = "set closeCout  [" + s + "] ok";

    return true;
}

void Application::outAllAdapter(ostream &os)
{
    map<int, TC_EpollServer::BindAdapterPtr> m = _epollServer->getListenSocketInfo();

    for(map<int, TC_EpollServer::BindAdapterPtr>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
        outAdapter(os, ServantHelperManager::getInstance()->getAdapterServant(it->second->getName()),it->second);

        os << OUT_LINE << endl;
    }
}

bool Application::addConfig(const string &filename)
{
    string result;

    if (TafRemoteConfig::getInstance()->addConfig(filename, result, false))
    {
        TafRemoteNotify::getInstance()->report(result);

        return true;
    }
    TafRemoteNotify::getInstance()->report(result);

    return true;
}

bool Application::addAppConfig(const string &filename)
{
    string result = "";

    // true-只获取应用级别配置
    if (TafRemoteConfig::getInstance()->addConfig(filename, result, true))
    {
        TafRemoteNotify::getInstance()->report(result);

        return true;
    }

    TafRemoteNotify::getInstance()->report(result);

    return true;
}

void Application::setHandle(TC_EpollServer::BindAdapterPtr& adapter)
{
    adapter->setHandle<ServantHandle>();
}

void Application::main(int argc, char *argv[])
{
    try
    {
        TC_Common::ignorePipe();

        //解析配置文件
        parseConfig(argc, argv);

        //初始化Proxy部分
        initializeClient();

        //初始化Server部分
        initializeServer();

        vector<TC_EpollServer::BindAdapterPtr> adapters;

        //绑定对象和端口
        bindAdapter(adapters);

        //业务应用的初始化
        initialize();

        //输出所有adapter
        outAllAdapter(cout);

        //设置HandleGroup分组，启动线程
        for (size_t i = 0; i < adapters.size(); ++i)
        {
            string name = adapters[i]->getName();

            string groupName = adapters[i]->getHandleGroupName();

            if(name != groupName)
            {
                TC_EpollServer::BindAdapterPtr ptr = _epollServer->getBindAdapter(groupName);

                if (!ptr)
                {
                    throw runtime_error("[TAF][adater `" + name + "` setHandle to group `" + groupName + "` fail!");
                }

            }
            setHandle(adapters[i]);

            /*
            if (name == groupName)
            {
                setHandle(adapters[i]);

                continue;
            }

            TC_EpollServer::BindAdapterPtr ptr = _epollServer->getBindAdapter(groupName);

            if (!ptr)
            {
                throw runtime_error("[TAF][adater `" + name + "` setHandle to group `" + groupName + "` fail!");
            }

            adapters[i]->setHandle(ptr);
            */
        }

        //启动业务处理线程
        _epollServer->startHandle();

        cout << "\n" << outfill("[initialize server] ", '.')  << " [Done]" << endl;

        cout << OUT_LINE_LONG << endl;

        //动态加载配置文件
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_LOAD_CONFIG, Application::cmdLoadConfig);

        //动态设置日志等级
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_SET_LOG_LEVEL, Application::cmdSetLogLevel);

        //查看服务状态
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_VIEW_STATUS, Application::cmdViewStatus);

        //查看当前链接状态
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_CONNECTIONS, Application::cmdConnections);

        //查看编译的TAF版本
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_VIEW_VERSION, Application::cmdViewVersion);

        //加载配置文件中的属性信息
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_LOAD_PROPERTY, Application::cmdLoadProperty);

        //查看服务支持的管理命令
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_VIEW_ADMIN_COMMANDS, Application::cmdViewAdminCommands);

        //设置染色信息
        TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_SET_DYEING, Application::cmdSetDyeing);

        //上报版本
        TAF_REPORTVERSION(TAF_VERSION);

        //发送心跳给node, 表示启动了
        TAF_KEEPALIVE("");

        //发送给notify表示服务启动了
        TafRemoteNotify::getInstance()->report("restart");

        //ctrl + c能够完美结束服务
        signal(SIGINT, sighandler);

      
        if(_conf.get("/taf/application/server<closecout>",AppCache::getInstance()->get("closeCout")) != "0")
        {
            // 重定向stdin、stdout、stderr
            int fd = open("/dev/null", O_RDWR );    
            if(fd != -1)
            {
               dup2(fd, 0);
               dup2(fd, 1);
               dup2(fd, 2); 
            }
            else
            {
               close(0);
               close(1);
               close(2); 
            }
        }
    }
    catch (exception &ex)
    {
        TafRemoteNotify::getInstance()->report("exit: " + string(ex.what()));

        cout << "[main exception]:" << ex.what() << endl;

        terminate();
    }

    //初始化完毕后, 日志再修改为异步
    TafRollLogger::getInstance()->sync(false);
}

void Application::parseConfig(int argc, char *argv[])
{
    TC_Option op;

    op.decode(argc, argv);

    //直接输出编译的TAF版本
    if(op.hasParam("version"))
    {
        cout << "TAF:" << TAF_VERSION << endl;
        exit(0);
    }

    //加载配置文件
    ServerConfig::ConfigFile = op.getValue("config");

    if(ServerConfig::ConfigFile == "")
    {
        cerr << "start server with config, for example: " << argv[0] << " --config=config.conf" << endl;

        exit(0);
    }

    _conf.parseFile(ServerConfig::ConfigFile);
}

TC_EpollServer::BindAdapter::EOrder Application::parseOrder(const string &s)
{
    vector<string> vtOrder = TC_Common::sepstr<string>(s,";, \t", false);

    if(vtOrder.size() != 2)
    {
        cerr << "invalid order '" << TC_Common::tostr(vtOrder) << "'."<< endl;
        
        exit(0);
    }
    if((TC_Common::lower(vtOrder[0]) == "allow")&&(TC_Common::lower(vtOrder[1]) == "deny"))
    {
        return TC_EpollServer::BindAdapter::ALLOW_DENY;
    }
    if((TC_Common::lower(vtOrder[0]) == "deny")&&(TC_Common::lower(vtOrder[1]) == "allow"))
    {
         return TC_EpollServer::BindAdapter::DENY_ALLOW;
    }
    
     cerr << "invalid order '" << TC_Common::tostr(vtOrder) << "'."<< endl;
             
     exit(0);    
}

void Application::initializeClient()
{
    cout << "\n" << OUT_LINE_LONG << endl;

    //初始化通信器
    _communicator = CommunicatorFactory::getInstance()->getCommunicator(_conf);
    
    cout << outfill("[proxy config]:") << endl;
    
    //输出
    outClient(cout);
}

void Application::outClient(ostream &os)
{
    os << outfill("locator")                     << _communicator->getProperty("locator") << endl;
    os << outfill("sync-invoke-timeout")         << _communicator->getProperty("sync-invoke-timeout") << endl;
    os << outfill("async-invoke-timeout")        << _communicator->getProperty("async-invoke-timeout") << endl;
    os << outfill("refresh-endpoint-interval")   << _communicator->getProperty("refresh-endpoint-interval") << endl;
    os << outfill("stat")                        << _communicator->getProperty("stat") << endl;
    os << outfill("property")                    << _communicator->getProperty("property") << endl;
    os << outfill("report-interval")             << _communicator->getProperty("report-interval") << endl;
    os << outfill("sample-rate")                 << _communicator->getProperty("sample-rate") << endl;
    os << outfill("max-sample-count")            << _communicator->getProperty("max-sample-count") << endl;
    os << outfill("sendthread")                  << _communicator->getProperty("sendthread") << endl;
    os << outfill("recvthread")                  << _communicator->getProperty("recvthread") << endl;
    os << outfill("asyncthread")                 << _communicator->getProperty("asyncthread") << endl;
    os << outfill("modulename")                  << _communicator->getProperty("modulename") << endl;
}

string Application::toDefault(const string &s, const string &sDefault)
{
    if(s.empty())
    {
        return sDefault;
    }
    return s;
}

void Application::addServantProtocol(const string& servant, const TC_EpollServer::protocol_functor& protocol)
{
    string adapterName = ServantHelperManager::getInstance()->getServantAdapter(servant);

    if (adapterName == "")
    {
        throw runtime_error("[TAF]addServantProtocol fail, no found adapter for servant:" + servant);
    }
    getEpollServer()->getBindAdapter(adapterName)->setProtocol(protocol);
}

void Application::initializeServer()
{
    cout << OUT_LINE << "\n" << outfill("[server config]:") << endl;

    ServerConfig::Application  = toDefault(_conf.get("/taf/application/server<app>"), "UNKNOWN");

    //缺省采用进程名称
    string exe = "";

    try
    {
        exe = TC_File::extractFileName(TC_File::getExePath());
    }
    catch(TC_File_Exception & ex)
    {
        //取失败则使用ip代替进程名
        exe = _conf.get("/taf/application/server<localip>");
    }

    ServerConfig::ServerName        = toDefault(_conf.get("/taf/application/server<server>"), exe);
    ServerConfig::BasePath          = toDefault(_conf.get("/taf/application/server<basepath>"), ".") + "/";
    ServerConfig::DataPath          = toDefault(_conf.get("/taf/application/server<datapath>"), ".") + "/";
    ServerConfig::LogPath           = toDefault(_conf.get("/taf/application/server<logpath>"),  ".") + "/";
    ServerConfig::LogSize           = TC_Common::toSize(toDefault(_conf.get("/taf/application/server<logsize>"), "52428800"), 52428800);
    ServerConfig::LogNum            = TC_Common::strto<int>(toDefault(_conf.get("/taf/application/server<lognum>"), "10"));
    ServerConfig::LocalIp           = _conf.get("/taf/application/server<localip>");
    ServerConfig::Local             = _conf.get("/taf/application/server<local>");
    ServerConfig::Node              = _conf.get("/taf/application/server<node>");
    ServerConfig::Log               = _conf.get("/taf/application/server<log>");
    ServerConfig::Config            = _conf.get("/taf/application/server<config>");
    ServerConfig::Notify            = _conf.get("/taf/application/server<notify>");
    ServerConfig::ReportFlow        = _conf.get("/taf/application/server<reportflow>")=="0"?0:1;

    if(ServerConfig::LocalIp.empty())
    {
        vector<string> v = TC_Socket::getLocalHosts();

        ServerConfig::LocalIp = "127.0.0.1";
        //获取第一个非127.0.0.1的IP
        for(size_t i = 0; i < v.size(); i++)
        {
            if(v[i] != "127.0.0.1")
            {
                ServerConfig::LocalIp = v[i];
                break;
            }
        }
    }

    //输出信息
    outServer(cout);

    _epollServer = new TC_EpollServer();

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地文件cache
    cout << OUT_LINE << "\n" << outfill("[set file cache ]") << "OK" << endl;
    AppCache::getInstance()->setCacheInfo(ServerConfig::DataPath+ServerConfig::ServerName+".tafdat",0);
        
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地Log
    cout << OUT_LINE << "\n" << outfill("[set roll logger] ") << "OK" << endl;
    TafRollLogger::getInstance()->setLogInfo(ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath, ServerConfig::LogSize, ServerConfig::LogNum, _communicator, ServerConfig::Log);
    _epollServer->setLocalLogger(TafRollLogger::getInstance()->logger());

    //初始化是日志为同步
    TafRollLogger::getInstance()->sync(true);

    //设置日志级别
    string level = AppCache::getInstance()->get("logLevel");

    if(!level.empty())
    {
        TafRollLogger::getInstance()->logger()->setLogLevel(level);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到LogServer代理
    cout << OUT_LINE << "\n" << outfill("[set time logger] ") << "OK" << endl;
    TafTimeLogger::getInstance()->setLogInfo(_communicator, ServerConfig::Log, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到配置中心代理
    cout << OUT_LINE << "\n" << outfill("[set remote config] ") << "OK" << endl;
    TafRemoteConfig::getInstance()->setConfigInfo(_communicator, ServerConfig::Config, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::BasePath);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到信息中心代理
    cout << OUT_LINE << "\n" << outfill("[set remote notify] ") << "OK" << endl;
    TafRemoteNotify::getInstance()->setNotifyInfo(_communicator, ServerConfig::Notify, ServerConfig::Application, ServerConfig::ServerName);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到Node的代理
    cout << OUT_LINE << "\n" << outfill("[set node proxy]") << "OK" << endl;
    TafNodeFHelper::getInstance()->setNodeInfo(_communicator, ServerConfig::Node, ServerConfig::Application, ServerConfig::ServerName);
       
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化管理对象
    cout << OUT_LINE << "\n" << outfill("[set admin adapter]") << "OK" << endl;

    if(!ServerConfig::Local.empty())
    {
        ServantHelperManager::getInstance()->addServant<AdminServant>("AdminObj");

        ServantHelperManager::getInstance()->setAdapterServant("AdminAdapter", "AdminObj");

        TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(_epollServer.get());

        lsPtr->setName("AdminAdapter");

        lsPtr->setEndpoint(ServerConfig::Local);

        lsPtr->setMaxConns(TC_EpollServer::BindAdapter::DEFAULT_MAX_CONN);

        lsPtr->setQueueCapacity(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_CAP);

        lsPtr->setQueueTimeout(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_TIMEOUT);

        lsPtr->setProtocolName("taf");

        lsPtr->setProtocol(AppProtocol::parse);

        lsPtr->setHandleGroupName("AdminAdapter");

        lsPtr->setHandleNum(1);

        lsPtr->setHandle<ServantHandle>();

        _epollServer->bind(lsPtr);
    }
}

void Application::outServer(ostream &os)
{
    os << outfill("Application")        << ServerConfig::Application << endl;
    os << outfill("ServerName")         << ServerConfig::ServerName << endl;
    os << outfill("BasePath")           << ServerConfig::BasePath << endl;
    os << outfill("DataPath")           << ServerConfig::DataPath << endl;
    os << outfill("LocalIp")            << ServerConfig::LocalIp << endl;
    os << outfill("Local")              << ServerConfig::Local << endl;
    os << outfill("LogPath")            << ServerConfig::LogPath << endl;
    os << outfill("LogSize")            << ServerConfig::LogSize << endl;
    os << outfill("LogNum")             << ServerConfig::LogNum << endl;
    os << outfill("Log")                << ServerConfig::Log << endl;
    os << outfill("Node")               << ServerConfig::Node << endl;
    os << outfill("Config")             << ServerConfig::Config << endl;
    os << outfill("Notify")             << ServerConfig::Notify << endl;
    os << outfill("CloseCout")          << TC_Common::tostr(_conf.get("/taf/application/server<closecout>",AppCache::getInstance()->get("closeCout")) == "0"?0:1)<< endl;
    os << outfill("ReportFlow")         << ServerConfig::ReportFlow<< endl;    
}

void Application::bindAdapter(vector<TC_EpollServer::BindAdapterPtr>& adapters)
{
    string sPrefix = ServerConfig::Application + "." + ServerConfig::ServerName + ".";

    vector<string> adapterName;

    map<string, ServantHandle*> servantHandles;

    if (_conf.getDomainVector("/taf/application/server", adapterName))
    {
        for (size_t i = 0; i < adapterName.size(); i++)
        {
            string servant = _conf.get("/taf/application/server/" + adapterName[i] + "<servant>");

            checkServantNameValid(servant, sPrefix);

            //TC_EpollServer::BindAdapterPtr bindAdapter = bind("/taf/application/server", adapterName[i], servant);

            ServantHelperManager::getInstance()->setAdapterServant(adapterName[i], servant);

            TC_EpollServer::BindAdapterPtr bindAdapter = new TC_EpollServer::BindAdapter(_epollServer.get());

            string sLastPath = "/taf/application/server/" + adapterName[i];

            bindAdapter->setName(adapterName[i]);

            bindAdapter->setEndpoint(_conf[sLastPath + "<endpoint>"]);

            bindAdapter->setMaxConns(TC_Common::strto<int>(_conf.get(sLastPath + "<maxconns>", "128")));

            bindAdapter->setOrder(parseOrder(_conf.get(sLastPath + "<order>","allow,deny")));

            bindAdapter->setAllow(TC_Common::sepstr<string>(_conf[sLastPath + "<allow>"], ";,", false));

            bindAdapter->setDeny(TC_Common::sepstr<string>(_conf.get(sLastPath + "<deny>",""), ";,", false));

            bindAdapter->setQueueCapacity(TC_Common::strto<int>(_conf.get(sLastPath + "<queuecap>", "1024")));

            bindAdapter->setQueueTimeout(TC_Common::strto<int>(_conf.get(sLastPath + "<queuetimeout>", "10000")));

            bindAdapter->setProtocolName(_conf.get(sLastPath + "<protocol>", "taf"));

            if (bindAdapter->isTafProtocol())
            {
                bindAdapter->setProtocol(AppProtocol::parse);
            }

            bindAdapter->setHandleGroupName(_conf.get(sLastPath + "<handlegroup>", adapterName[i]));

            bindAdapter->setHandleNum(TC_Common::strto<int>(_conf.get(sLastPath + "<threads>", "0")));
            
            bindAdapter->setShm(TC_Common::strto<int>(_conf.get(sLastPath + "<shmkey>", "0")), TC_Common::strto<int>(_conf.get(sLastPath + "<shmcap>", "0")));
            
            bindAdapter->initialize();

            _epollServer->bind(bindAdapter);

            adapters.push_back(bindAdapter);

            //队列取平均值
            if(!_communicator->getProperty("property").empty())
            {
                _communicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".queue", PropertyReport::avg());
            }
        }
    }
}

void Application::checkServantNameValid(const string& servant, const string& sPrefix)
{
    if((servant.length() <= sPrefix.length()) || (servant.substr(0, sPrefix.length()) != sPrefix))
    {
        ostringstream os;

        os << "Servant '" << servant << "' error: must be start with '" << sPrefix << "'";

        TafRemoteNotify::getInstance()->report("exit:" + os.str());

        cout << os.str() << endl;

        terminate();

        exit(-1);
    }
}

void Application::outAdapter(ostream &os, const string &v, TC_EpollServer::BindAdapterPtr lsPtr)
{
    os << outfill("name")             << lsPtr->getName() << endl;
    os << outfill("servant")          << v << endl;
    os << outfill("endpoint")         << lsPtr->getEndpoint().toString() << endl;
    os << outfill("maxconns")         << lsPtr->getMaxConns() << endl;
    os << outfill("queuecap")         << lsPtr->getQueueCapacity() << endl;
    os << outfill("queuetimeout")     << lsPtr->getQueueTimeout() << "ms" << endl;
    os << outfill("order")            << (lsPtr->getOrder()==TC_EpollServer::BindAdapter::ALLOW_DENY?"allow,deny":"deny,allow") << endl;
    os << outfill("allow")            << TC_Common::tostr(lsPtr->getAllow()) << endl;
    os << outfill("deny")             << TC_Common::tostr(lsPtr->getDeny()) << endl;
    os << outfill("queuesize")        << lsPtr->getRecvBufferSize() << endl;
    os << outfill("connections")      << lsPtr->getNowConnection() << endl;
    os << outfill("protocol")         << lsPtr->getProtocolName() << endl;
    os << outfill("handlegroup")      << lsPtr->getHandleGroupName() << endl;
    os << outfill("handlethread")     << lsPtr->getHandleNum() << endl;
    os << outfill("shmkey")           << lsPtr->getShmKey() << endl;
    os << outfill("shmcap")           << lsPtr->getShmSize() << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
}
