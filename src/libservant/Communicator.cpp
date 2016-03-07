#include "util/tc_file.h"
#include "servant/Communicator.h"
#include "servant/StatReport.h"
#include "log/taf_logger.h"

namespace taf
{

string ClientConfig::LocalIp = "127.0.0.1";

string ClientConfig::ModuleName = "unknown";
//////////////////////////////////////////////////////////////////////////////////////////////
Communicator::Communicator()
: _initialized(false)
, _terminating(false)
, _timeoutHandle(NULL)
{
}

Communicator::Communicator(TC_Config& conf, const string& domain/* = CONFIG_ROOT_PATH*/)
: _initialized(false)
, _terminating(false)
, _timeoutHandle(NULL)
{
    try
    {
        setProperty(conf, domain);
    }
    catch (exception& e)
    {
        LOG->error() << "[TAF][Communicator fail:" << e.what() << "]" << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][Communicator fail.]" << endl;
    }
}

Communicator::~Communicator()
{
    terminate();

    delete _timeoutHandle;

    _timeoutHandle = NULL;

    LOG->debug() << "[TAF][~Communicator, communicator can only be destroyed when application exit.]" << endl;
}

bool Communicator::isTerminating() 
{
    return _terminating;
}

void Communicator::setProperty(TC_Config& conf, const string& domain/* = CONFIG_ROOT_PATH*/)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    try
    {
        conf.getDomainMap(domain, _properties);

        initClientConfig();
    }
    catch (exception& e)
    {
        LOG->error() << "[TAF][communicator setProperty fail:" << e.what() << "]" << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][communicator setProperty fail.]" << endl;
    }
}

void Communicator::initClientConfig()
{
    ClientConfig::LocalIp = getProperty("localip", "127.0.0.1");

    if (ClientConfig::LocalIp.empty())
    {
        vector<string> v = TC_Socket::getLocalHosts();

        //获取第一个非127.0.0.1的IP
        for(size_t i = 0; i < v.size(); i++)
        {
            if(v[i] != "127.0.0.1")
            {
                ClientConfig::LocalIp = v[i];
                break;
            }
        }
    }

    //缺省采用进程名称
    string exe = "";

    try
    {
        exe = TC_File::extractFileName(TC_File::getExePath());
    }
    catch(TC_File_Exception & ex)
    {
        //取失败则使用ip代替进程名
        exe = ClientConfig::LocalIp;
    }

    ClientConfig::ModuleName = getProperty("modulename", exe);
}

void Communicator::setProperty(const map<string, string>& properties)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _properties = properties;

    initClientConfig();
}

void Communicator::setProperty(const string& name, const string& value)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _properties[name] = value;

    initClientConfig();
}

string Communicator::getProperty(const string& name, const string& dft/* = ""*/)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    map<string, string>::iterator it = _properties.find(name);

    if (it != _properties.end())
    {
        return it->second;
    }
    return dft;
}

void Communicator::initialize()
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    if (_initialized)
    {
        return;
    }
    _initialized = true;

    _objectProxyFactory = new ObjectProxyFactory(this);

    _servantProxyFactory = new ServantProxyFactory(this);

    _transceiverHandle = new TransceiverHandle(this);

    //_transceiverHandle->start();

    _fdReactor = new FDReactor();

    _fdReactor->start();

    _tmReactor = new TMReactor();

    _timeoutHandle = new TimeoutHandle<TC_TimeoutQueue<ReqMessagePtr> >();

    _tmReactor->registerHandle(1, _timeoutHandle);

    _tmReactor->start();

    //初始化异步回调线程
    _asyncProcThread = new AsyncProcThread(this);

    int asyncThreadNum = TC_Common::strto<int>(getProperty("asyncthread", "3"));

    _asyncProcThread->start(asyncThreadNum);

    //初始化统计上报接口
    string statObj = getProperty("stat", "");

    string propertyObj = getProperty("property", "");  

    int iReportInterval = TC_Common::strto<int>(getProperty("report-interval", "60000"));
    
    int iSampleRate = TC_Common::strto<int>(getProperty("sample-rate", "1000"));
    
    int iMaxSampleCount = TC_Common::strto<int>(getProperty("max-sample-count", "100"));

    //stat总是有对象, 保证getStat返回的对象总是有效
    _statReportPtr = new StatReport();

    StatFPrx statPrx = NULL;

    if (!statObj.empty())
    {
        statPrx = stringToProxy<StatFPrx>(statObj);
    }

    //上报Property信息的代理
    PropertyFPrx propertyPrx = NULL;

    if (!propertyObj.empty())
    {
        propertyPrx = stringToProxy<PropertyFPrx>(propertyObj);
    }
    
    _statReportPtr->setReportInfo(statPrx, propertyPrx, ClientConfig::ModuleName, ClientConfig::LocalIp, iReportInterval, iSampleRate, iMaxSampleCount);

    if(!getProperty("property").empty() && !getProperty("modulename").empty())
    {
        this->getStatReport()->createPropertyReport(getProperty("modulename") + ".asyncqueue", PropertyReport::avg());
    }
}

int Communicator::reloadProperty(string & sResult)
{
    _objectProxyFactory->loadObjectLocator();

    int iReportInterval = TC_Common::strto<int>(getProperty("report-interval", "60000"));
    
    int iSampleRate = TC_Common::strto<int>(getProperty("sample-rate", "1000"));
    
    int iMaxSampleCount = TC_Common::strto<int>(getProperty("max-sample-count", "100"));
    
    string statObj = getProperty("stat", "");

    string propertyObj = getProperty("property", "");

    StatFPrx statPrx = NULL;

    if (!statObj.empty())
    {
        statPrx = stringToProxy<StatFPrx>(statObj);
    }

    PropertyFPrx propertyPrx = NULL;

    if (!propertyObj.empty())
    {
        propertyPrx = stringToProxy<PropertyFPrx>(propertyObj);
    }

    _statReportPtr->setReportInfo(statPrx, propertyPrx, ClientConfig::ModuleName, ClientConfig::LocalIp, iReportInterval,iSampleRate,iMaxSampleCount);

    sResult = "locator=" + getProperty("locator", "") + "\r\n" + 
              "stat=" + statObj + "\r\n" + "property=" + propertyObj + "\r\n" + 
              "report-interval=" + TC_Common::tostr(iReportInterval) + "\r\n"+ 
              "sample-rate=" + TC_Common::tostr(iSampleRate) + "\r\n"+ 
              "max-sample-count=" + TC_Common::tostr(iMaxSampleCount) + "\r\n";
    return 0;
}

void Communicator::terminate()
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _terminating = true;

    if (_initialized)
    {
        if (_tmReactor && _tmReactor->isAlive())
        {
            _tmReactor->terminate();

            _tmReactor->getThreadControl().join();
        }

        if (_asyncProcThread)
        {
            _asyncProcThread->terminate();
        }
/*
        if (_transceiverHandle)
        {
            _transceiverHandle->terminate();

            _transceiverHandle->getThreadControl().join();
        }
*/
        if (_fdReactor && _fdReactor->isAlive())
        {
            _fdReactor->terminate();

            _fdReactor->getThreadControl().join();
        }
    }
}

vector<TC_Endpoint> Communicator::getEndpoint(const string & objName)
{
    return _objectProxyFactory->getObjectProxy(objName)->getEndpoint();
}

vector<TC_Endpoint> Communicator::getEndpoint4All(const string & objName)
{
    return _objectProxyFactory->getObjectProxy(objName)->getEndpoint4All();
}

ServantPrx Communicator::getServantProxy(const string& objectName)
{
    Communicator::initialize();

    return _servantProxyFactory->getServantProxy(objectName);
}

StatReport* Communicator::getStatReport()
{
    Communicator::initialize();

    return _statReportPtr.get();
}

//protected:

FDReactor* Communicator::fdReactor()
{
    return _fdReactor.get();
}

ObjectProxyFactory* Communicator::objectProxyFactory()
{
    return _objectProxyFactory.get();
}

TransceiverHandle* Communicator::transceiverHandle()
{
    return _transceiverHandle.get();
}

AsyncProcThread* Communicator::asyncProcThread()
{
    return _asyncProcThread.get();
}

ServantProxyFactory* Communicator::servantProxyFactory()
{
    return _servantProxyFactory.get();
}

TimeoutHandle<TC_TimeoutQueue<ReqMessagePtr> >* Communicator::getTimeoutHandle()
{
    return _timeoutHandle;
}
///////////////////////////////////////////////////////////////
}
