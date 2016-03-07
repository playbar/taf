#include "servant/ObjectProxy.h"
#include "servant/QueryEndpoint.h"
#include "servant/Global.h"
#include "servant/AppCache.h"
#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "log/taf_logger.h"

namespace taf
{
///////////////////////////////////////////////////////////////////////////////////
AdapterProxyGroup::AdapterProxyGroup() 
: lastRoundPosition(0) 
{
}

AdapterPrx AdapterProxyGroup::getHashProxy(int64_t hashCode)
{
    if (!adapterProxys.empty())
    {
        return adapterProxys[((uint32_t)hashCode % adapterProxys.size())];
    }
    return NULL;
}

AdapterPrx AdapterProxyGroup::getRandomProxy()
{
    if (!adapterProxys.empty())
    {
        return adapterProxys[((uint32_t)rand() % adapterProxys.size())];
    }
    return NULL;
}

AdapterPrx AdapterProxyGroup::getNextValidProxy()
{
    int32_t all = (int32_t)adapterProxys.size();

    while (all-- > 0)
    {
        ++lastRoundPosition;

        lastRoundPosition %= adapterProxys.size();

        if (adapterProxys[lastRoundPosition]->checkActive() == true)
        {
            return adapterProxys[lastRoundPosition];
        }
    }
    return NULL;
}
///////////////////////////////////////////////////////////////////////////////////
ObjectProxy::ObjectProxy(Communicator* comm, const string& name)
: BaseProxy(comm)
, _name(name)
, _timeout(5*1000)
, _maxTransNum(1)
, _isDirectProxy(false)
, _serverHasGrid(false)
, _lastRefreshEndpointTime(0)
, _refreshEndpointInterval(60*1000)
, _lastCacheEndpointTime(0)
, _cacheEndpointInterval(30*60*1000)
, _roundStartIndex(0)
, _timeoutQueue(NULL)
, _queryEndpoint(NULL)
, _lastRefreshEndpoint4AllTime(0)
{
    _proxyProtocol.requestFunc = ProxyProtocol::tafRequest;

    _proxyProtocol.responseFunc = ProxyProtocol::tafResponse;
}

ObjectProxy::~ObjectProxy()
{
    if (_timeoutQueue != NULL && _comm->getTimeoutHandle())
    {
        _comm->getTimeoutHandle()->unregisterHandle(_timeoutQueue);
    }
    delete _timeoutQueue;

    _timeoutQueue = NULL;
/*
    if( _queryEndpoint != NULL)
    {
        delete _queryEndpoint;
    }
    _queryEndpoint = NULL;
    */
}

void ObjectProxy::initialize()
{
    int size = TC_Common::strto<int>(_comm->getProperty("timeout-queue-size", "50000"));

    _timeoutQueue = new TC_TimeoutQueue<ReqMessagePtr>(_timeout,size); //默认hash_map size设置为5W

    _comm->getTimeoutHandle()->registerHandle(_timeoutQueue);

    string::size_type n = _name.find_first_of('@');
    
    string endpoints;
    
    string name = _name;

    //[直接连接]指定服务的IP和端口列表
    if (n != string::npos)
    {
        _name = name.substr(0, n);

        _isDirectProxy = true;

        endpoints = name.substr(n + 1);
    }
    //[间接连接]通过registry查询服务端的IP和端口列表
    //[间接连接] 第一次使用cache
    else
    {          
        _locator = _comm->getProperty("locator");
        
        if (_locator.find_first_not_of('@') == string::npos)
        {
            LOG->error() << "[Locator is not valid:" << _locator << "]" << endl;

            throw TafRegistryException("locator is not valid:" + _locator);
        }
        QueryFPrx prx = _comm->stringToProxy<QueryFPrx>(_locator);

        _queryEndpoint = new taf::QueryEndpoint(prx);

        endpoints = AppCache::getInstance()->get(_name,_locator);        
    }
        
    vector<string> eps = TC_Common::sepstr<string>(endpoints, ":", false);
    
    for (uint32_t i = 0; i < eps.size(); ++i)
    {
        try
        {
            TC_Endpoint ep(eps[i]);

            EndpointInfo::EType type = (ep.isTcp() ? EndpointInfo::TCP : EndpointInfo::UDP);

            EndpointInfo epi(ep.getHost(), ep.getPort(), type, ep.getGrid());

            _activeEndpoints.insert(epi);

            AdapterPrx ap = new AdapterProxy(_comm, epi, this, _comm->fdReactor(), _comm->transceiverHandle());

            _adapterGroups[epi.grid()].adapterProxys.push_back(ap);
        }
        catch (...)
        {
            LOG->error() << "[endpoints parse error:" << name << ":" << eps[i] << "]" << endl;
        }
    }
    _serverHasGrid = (_adapterGroups.size() > 1 ? true : false);
}

int ObjectProxy::loadLocator()
{
    if(_isDirectProxy)
    {
        //直接连接
        return 0;
    }

    string locator = _comm->getProperty("locator");

    if (locator.find_first_not_of('@') == string::npos)
    {
        LOG->error() << "[Locator is not valid:" << locator << "]" << endl;

        return -1;
    }

    QueryFPrx prx = _comm->stringToProxy<QueryFPrx>(locator);

    ((QueryEndpoint*)_queryEndpoint)->setLocatorPrx(prx);

    return 0;
}

ServantProxyCallback* ObjectProxy::getOneRandomPushCallback()
{
    if (!_pushCallbacks.empty())
    {
        return _pushCallbacks[((uint32_t)rand() % _pushCallbacks.size())].get();
    }
    return NULL;
}

void ObjectProxy::setPushCallbacks(const ServantProxyCallbackPtr& cb)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    _pushCallbacks.push_back(cb);
}

const string& ObjectProxy::name() const
{
    return _name;
}

int ObjectProxy::timeout() const
{
    return _timeout;
}

void ObjectProxy::setAdapterMaxTransNum(int num)
{
    _maxTransNum = num < 1 ? 1 : num;
}

int ObjectProxy::getAdapterMaxTransNum() const
{
    return _maxTransNum;
}

void ObjectProxy::setProxyProtocol(const ProxyProtocol& protocol)
{
    _proxyProtocol = protocol;
}

ProxyProtocol& ObjectProxy::getProxyProtocol()
{
    return _proxyProtocol;
}

CheckTimeoutInfo& ObjectProxy::checkTimeoutInfo()
{
    return _checkTimeoutInfo;
}

void ObjectProxy::timeout(int msec)
{
    //保护，异步超时时间不能小于1秒
    if (msec >= 1000)
    {
        _timeout = msec;
    }
}

vector<TC_Endpoint> ObjectProxy::getEndpoint()
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    refreshEndpointInfos();

    vector<TC_Endpoint>  v;

    for(set<EndpointInfo>::iterator it = _activeEndpoints.begin(); it != _activeEndpoints.end(); ++it)
    {
       TC_Endpoint ep(it->host(),it->port(),_timeout,it->type() == EndpointInfo::TCP?1:0,it->grid());

       v.push_back(ep);
    }
    return v;
}


vector<TC_Endpoint> ObjectProxy::getEndpoint4All()
{
    //直连 直接返回初始化时候的列表
    if (_isDirectProxy)
    {
        return getEndpoint();
    }

    time_t now = TC_TimeProvider::getInstance()->getNow();

    TC_LockT<TC_ThreadMutex> lock(*this);

     //如果是间接连接，通过registry查询服务列表
    if ((_lastRefreshEndpoint4AllTime + _refreshEndpointInterval/1000 < now) ||
        (_activeEndpoints4All.empty() && (_lastRefreshEndpoint4AllTime + 2 < now))) //2s保护策略
    {
        _lastRefreshEndpoint4AllTime = now;

        set<EndpointInfo>  activeEndpoints = ((QueryEndpoint*)_queryEndpoint)->findObjectById(_name);
        if (!activeEndpoints.empty())
        {
            _activeEndpoints4All = activeEndpoints;
        } 
    }
    vector<TC_Endpoint>  v;  
    for(set<EndpointInfo>::iterator it = _activeEndpoints4All.begin(); it != _activeEndpoints4All.end(); ++it)
    {
       TC_Endpoint ep(it->host(),it->port(),_timeout,it->type() == EndpointInfo::TCP?1:0,it->grid());

       v.push_back(ep);
    }
    return v;
}

void ObjectProxy::getEndpoint4All(const std::string & sStation, vector<TC_Endpoint> & vecActive, vector<TC_Endpoint> & vecInactive)
{
    vecActive.clear();
    vecInactive.clear();
    //直连 直接返回初始化时候的列表
    if (_isDirectProxy)
    {
        vecActive = getEndpoint();
        return ;
    }

    time_t now = TC_TimeProvider::getInstance()->getNow();

    TC_LockT<TC_ThreadMutex> lock(*this);
    std::map<std::string, std::pair<std::vector<TC_Endpoint>, std::vector<TC_Endpoint> > >::iterator itEndpoint = _mapEndpoints.find(sStation);
    if ((_lastRefreshEndpoint4AllTime + _refreshEndpointInterval/1000 < now) || (itEndpoint == _mapEndpoints.end() && (_lastRefreshEndpoint4AllTime + 2 < now)))
    {
        std::set<EndpointInfo> setActive, setInactive;
        ((QueryEndpoint*)_queryEndpoint)->findObjectByStation(_name, sStation, setActive, setInactive);

        for(std::set<EndpointInfo>::iterator it = setActive.begin(); it != setActive.end(); ++it)
        {
            TC_Endpoint ep(it->host(), it->port(), _timeout, it->type() == EndpointInfo::TCP?1:0, it->grid());

            vecActive.push_back(ep);
        }

        for(std::set<EndpointInfo>::iterator it = setInactive.begin(); it != setInactive.end(); ++it)
        {
            TC_Endpoint ep(it->host(), it->port(), _timeout, it->type() == EndpointInfo::TCP?1:0, it->grid());

            vecInactive.push_back(ep);
        }

        _mapEndpoints[sStation].first = vecActive;
        _mapEndpoints[sStation].second= vecInactive;

        return ;
    }

    vecActive   = _mapEndpoints[sStation].first;
    vecInactive = _mapEndpoints[sStation].second;
}

int ObjectProxy::refreshEndpointInterval() const
{
    return _refreshEndpointInterval;
}

void ObjectProxy::refreshEndpointInterval(int msecond)
{
    //保护，刷新服务端列表的时间不能小于1秒
    _refreshEndpointInterval = msecond > 1000 ? msecond : _refreshEndpointInterval;
}

void ObjectProxy::cacheEndpointInterval(int msecond)
{
    //保护，cache服务端列表的时间不能小于60秒 默认30分钟
    _cacheEndpointInterval = msecond > 1000*60 ? msecond : _cacheEndpointInterval;
}

TC_TimeoutQueue<ReqMessagePtr>* ObjectProxy::getTimeoutQueue()
{
    return _timeoutQueue;
}

int ObjectProxy::invoke(ReqMessagePtr& req)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    //选择一个远程服务的Adapter来调用
    AdapterPrx adp = selectAdapterProxy(req);

    if (!adp)
    {
        LOG->error() << "[TAF][invoke, " << _name << ", select adapter proxy ret NULL]" << endl;

        return -2;
    }

    //注意这里选出了adapter,但是对于非hash和非灰度的请求,并不一定是这个adapter发送出去的!注意哦
    return adp->invoke(req);
}

//响应消息在AdapterProxy层已经全部处理完
int ObjectProxy::finished(ReqMessagePtr& req)
{
    return 0;
}

//由后续的AdapterProxy取出一个消息发送
bool ObjectProxy::popRequest(ReqMessagePtr& req)
{
    //从FIFO队列中获取一条消息进行发送
    ReqMessagePtr ptr = _timeoutQueue->pop();

    if (ptr)
    {
        req = ptr;

        return true;
    }
    return false;
}

//定时刷新服务列表
void ObjectProxy::refreshEndpointInfos()
{
    try
    {
        //如果是直连的，则用初始化时候的服务列表
        //因为不会删除无效的AdapterProxy，所以不用刷新
        if (_isDirectProxy)
        {
            return;
        }

        time_t now = TC_TimeProvider::getInstance()->getNow();

        //如果是间接连接，通过registry定时查询服务列表
        if ((_lastRefreshEndpointTime + _refreshEndpointInterval/1000 < now) ||
            (_adapterGroups.empty() && (_lastRefreshEndpointTime + 2 < now))) //2s保护策略
        {
            _lastRefreshEndpointTime = now;

            //初始化时使用同步调用，以后用异步刷新
            bool sync = (_activeEndpoints.empty() || (_lastCacheEndpointTime == 0));

            ((QueryEndpoint*)_queryEndpoint)->findObjectById4All(_name, sync);
        }

        //不加锁判断是否已经有刷新
        if (!((QueryEndpoint*)_queryEndpoint)->hasRefreshed())
        {
            return;
        }

        set<EndpointInfo> activeEps;

        set<EndpointInfo> inactiveEps;

        set<int32_t> allGridCodes;

        //异步调用还没有callback回来
        if (!((QueryEndpoint*)_queryEndpoint)->hasNewEndpoints(activeEps, inactiveEps, allGridCodes))
        {
            LOG->info() << "[TAF][refreshEndpointInfos,`findObjectById4All` hasNewEndpoints false:" << _name << "]" << endl;

            return;
        }

        //如果registry返回Active服务列表为空，不做更新
        if (activeEps.empty())
        {
            LOG->error() << "[TAF][refreshEndpointInfos,`findObjectById4All` ret activeEps is empty:" << _name << "]" << endl;

            return;
        }

        set<EndpointInfo> del; //需要删除的服务地址

        set<EndpointInfo> add; //需要增加的服务地址

        set_difference(_activeEndpoints.begin(), _activeEndpoints.end(), activeEps.begin(), activeEps.end(), inserter(del,del.begin()));

        set_difference(activeEps.begin(), activeEps.end(), _activeEndpoints.begin(), _activeEndpoints.end(), inserter(add,add.begin()));

        //如果服务端只有一种状态，则忽略路由设置
        _serverHasGrid = (allGridCodes.size() > 1 ? true : false);

        //没有需要更新的服务节点
        if (del.empty() && add.empty())
        {
            return;
        }

        LOG->info() << "[TAF][refreshEndpointInfos," << _name << ":`findObjectById4All` add:" << add.size() << ",del:" << del.size() << ",isGrid:" << _serverHasGrid << "]" << endl;

        //删除本地无效的服务节点
        for (set<EndpointInfo>::iterator it = del.begin(); it != del.end(); ++it)
        {
            map<int32_t, AdapterProxyGroup>::iterator mit = _adapterGroups.find(it->grid());

            if (mit == _adapterGroups.end())
            {
                continue;
            }
            vector<AdapterPrx>::iterator vit = mit->second.adapterProxys.begin();

            while (vit != mit->second.adapterProxys.end())
            {
                if ((*it) == (*vit)->endpoint())
                {
                    mit->second.adapterProxys.erase(vit);

                    continue;
                }
                ++vit;
            }
        }

        //添加本地还没有的节点
        for (set<EndpointInfo>::iterator it = add.begin(); it != add.end(); ++it)
        {
            AdapterPrx ap = new AdapterProxy(_comm, *it, this, _comm->fdReactor(), _comm->transceiverHandle());

            _adapterGroups[it->grid()].adapterProxys.push_back(ap);
        }

        //将最新的列表保存起来，下次刷新时使用
        _activeEndpoints.clear();

        for (map<int32_t, AdapterProxyGroup>::iterator it = _adapterGroups.begin(); it != _adapterGroups.end(); ++it)
        {
            for (uint32_t i = 0; i < it->second.adapterProxys.size(); ++i)
            {
                _activeEndpoints.insert(it->second.adapterProxys[i]->endpoint());
            }
        }     
    }
    catch (exception& e)
    {
        LOG->error() << "[TAF][refreshEndpointInfos,`findObjectById4All` exception:" << _name << ":" << e.what() << "]" << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][refreshEndpointInfos,`findObjectById4All` exception:" << _name << "]" << endl;
    }
}

//从指定的一组adapter中选取一个有效的
AdapterPrx ObjectProxy::selectFromGroup(const ReqMessagePtr& req, AdapterProxyGroup& group)
{
    AdapterPrx prx = NULL;

    //如果有hash，则先使用hash策略
    if (req->hashCode != ThreadPrivateData::INVALID_HASH_CODE)
    {
        prx = group.getHashProxy(req->hashCode);

        if (prx && prx->checkActive() == true)
        {
            return prx;
        }
    }
    //没有hash或者hash失败，则轮循选下一个有效的
    prx = group.getNextValidProxy();

    if (!prx)
    {
        //没有有效的服务，则随机选取一个(发送时会尝试重连)
        prx = group.getRandomProxy();
    }
    return prx;
}
/******************************************************************
 * 从可用的服务列表选择一个服务节点
 */
AdapterPrx ObjectProxy::selectAdapterProxy(const ReqMessagePtr& req)
{
    refreshEndpointInfos();

    //没有可用的服务列表，直接返回NULL，业务会收到异常
    if (_adapterGroups.empty())
    {
        LOG->error() << "[TAF][selectAdapterProxy," << _name << ",adapter proxy groups is empty!]" << endl;

        return NULL;
    }
    
    time_t now = TC_TimeProvider::getInstance()->getNow();

    //非直连的需要定时更新缓存服务列表
    if (!_isDirectProxy && _lastCacheEndpointTime + _cacheEndpointInterval/1000 < now)
    {
        _lastCacheEndpointTime = now;

        string sEndpoints = "";

        for (set<EndpointInfo>::iterator it = _activeEndpoints.begin(); it != _activeEndpoints.end(); ++it)
        {
            bool isTcp = (it->type() == EndpointInfo::TCP ? true : false);

            TC_Endpoint ep(it->host(), it->port(), _timeout, isTcp, it->grid());

            if (sEndpoints != "") sEndpoints += ":";

            sEndpoints += ep.toString();
        }

        AppCache::getInstance()->set(_name,sEndpoints,_locator);

        LOG->info() << "[TAF][setEndPointToCache,obj:" << _name << ",endpoint:" << sEndpoints << "]" << endl;
    }

    int32_t gridCode = 0;

    bool isValidGrid = false;

    string gridKey = "";

    const map<string, string>& status = req->request.status;

    //如果是灰度路由消息，检测其有效性
    if (IS_MSG_TYPE(req->request.iMessageType, taf::JCEMESSAGETYPEGRID))
    {
        map<string, string>::const_iterator keyIt = status.find(ServantProxy::STATUS_GRID_KEY);

        map<string, string>::const_iterator codeIt = status.find(ServantProxy::STATUS_GRID_CODE);

        if (keyIt != status.end() && codeIt != status.end())
        {
            gridCode = TC_Common::strto<int32_t>(codeIt->second);

            if (gridCode != ThreadPrivateData::INVALID_GRID_CODE)
            {
                isValidGrid = true;
            }
        }
    }

    //有效的路由消息，且服务端有多种状态
    if (isValidGrid == true && _serverHasGrid == true)
    {
        map<int32_t, AdapterProxyGroup>::iterator it = _adapterGroups.find(gridCode);

        if (it != _adapterGroups.end())
        {
            return selectFromGroup(req, it->second);
        }

        LOG->error() << "[TAF][selectAdapterProxy," << _name << ",grid router fail,gridKey:" << gridKey << "->gridCode:" << gridCode << "]" << endl;

        return NULL;
    }

    //非路由消息或服务端只有一种状态(有可能引起reset响应)
    return selectFromGroup(req, _adapterGroups.begin()->second);
}

/*************************************************************************************
 * 灰度路由发送到服务端，服务端检测如果状态不匹配，则返回reset消息和正确的状态
 * 客户端将该服务的adapter转移到正确的group，但是不会改变本地adapter的endpoint数据
 * 注意：如果服务端检测状态不匹配，但是服务端状态为0，则不会返回reset
 */
bool ObjectProxy::resetAdapterGrid(int32_t gridFrom, int32_t gridTo, AdapterProxy* adapter)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    LOG->info() << "[TAF][resetAdapterGrid," << _name << ",reset adapter grid:" << gridFrom << "->" << gridTo <<":" << adapter->endpoint().desc() << "]" << endl;

    map<int32_t, AdapterProxyGroup>::iterator it = _adapterGroups.find(gridFrom);

    if (gridFrom == gridTo || it == _adapterGroups.end())
    {
        return false;
    }

    vector<AdapterPrx>& vap = it->second.adapterProxys;

    for (vector<AdapterPrx>::iterator vit = vap.begin(); vit != vap.end(); ++vit)
    {
        if ((*vit)->endpoint() == adapter->endpoint())
        {
            //放入正确的group
            _adapterGroups[gridTo].adapterProxys.push_back(*vit);

            //在现有的group中删除
            vap.erase(vit);

            _serverHasGrid = (_adapterGroups.size() > 1 ? true : false);

            return true;
        }
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////////////
}
