#include "servant/Servant.h"
#include "servant/BaseF.h"
#include "servant/Application.h"
#include "log/taf_logger.h"

#include <cerrno>

namespace taf
{

Servant::Servant()
{
}

Servant::~Servant() 
{
}

void Servant::setName(const string &name) 
{ 
    _name = name; 
}

string Servant::getName() const 
{ 
    return _name; 
}

void Servant::setHandle(TC_EpollServer::Handle* handle) 
{ 
    _handle = handle; 
}

TC_EpollServer::Handle* Servant::getHandle() 
{ 
    return _handle; 
}

int Servant::dispatch(JceCurrentPtr current, vector<char> &buffer)
{
    int ret = JCESERVERUNKNOWNERR;

    if (current->getFuncName() == "taf_ping")
    {
        LOG->info() << "[TAF][Servant::dispatch] taf_ping ok from [" << current->getIp() << ":" << current->getPort() << "]" << endl;

        ret = JCESERVERSUCCESS;
    }
    else if (!current->getBindAdapter()->isTafProtocol())
    {
        TC_LockT<TC_ThreadRecMutex> lock(*this);

        ret = doRequest(current, buffer);
    }
    else
    {
        TC_LockT<TC_ThreadRecMutex> lock(*this);

        ret = onDispatch(current, buffer);
    }
    return ret;
}

/*
服务端路由
*/
int Servant::doGridRouter(JceCurrentPtr current, string & routekey)
{
    try
    {
        LOG->info() << "[TAF] Servant::doGridRouter got a grid req, key:" << routekey << endl;

        TC_Endpoint point   = current->getBindAdapter()->getEndpoint(); 
        //当前服务是否是路由服务
        int32_t serverGrid = point.getGrid();
    
        //如果业务设置了路由器，使用路由器
        int32_t grid = ServantHelperManager::getInstance()->getGridByKey(current->getServantName(),routekey);
    
        if(grid == ThreadPrivateData::INVALID_GRID_CODE || grid == serverGrid)
        {
             return -1;
        }
    
        //检测是否来自自己(服务端adapter与通信器维护的grid是否相同  防止不一致导致灰度死循环)
        if(current->getIp() == point.getHost() && current->getPort() == point.getPort())
        {
            return  -1;
        }
    
        ServantProxyThreadData* sptd = ServantProxyThreadData::getData();
    
        if (sptd)
        {
            sptd->data()->_routeKey = routekey;
    
            sptd->data()->_gridCode = grid;
        }
    
        ServantPrx pPrx = Application::getCommunicator()->stringToProxy<ServantPrx>(current->getServantName());
    
        if(current->getPacketType() == JCENORMAL)
        {
            ResponsePacket rep;
            map<string,string> status = current->getRequestStatus();
            status.insert(std::make_pair(ServantProxy::STATUS_GRID_KEY, routekey));
    
            pPrx->taf_invoke(current->getPacketType(),current->getFuncName(),current->getRequestBuffer(),
                             current->getContext(),status,rep);
            current->sendResponse(rep.iRet, rep.sBuffer,rep.status,rep.sResultDesc);
            current->setResponse(false);
        }
        else 
        {
            ServantCallbackPtr p = NULL;
            if (current->getMessageType() & JCEMESSAGETYPEASYNC)
            {
                p = new RouterCallback("RouterCallback",this,current);
            }
            pPrx->taf_invoke_async(current->getPacketType(),current->getFuncName(),current->getRequestBuffer(),
                             current->getContext(),current->getRequestStatus(),p);
            current->setResponse(false);
        }
        return 0;
    }
    catch(exception &ex)
    {
        LOG->error() << "[TAF]Servant::doGridRouter " << ex.what() << endl;
    }
    catch(...)
    {
        LOG->error() << "[TAF]Servant::doGridRouter unknown error" << endl;
    }
    return -1;
}


TC_ThreadQueue<ReqMessagePtr>& Servant::getResponseQueue() 
{ 
    return _asyncResponseQueue; 
}

///////////////////////////////////////////////////////////////////////////
ServantCallback::ServantCallback(const string& type, const ServantPtr& servant, const JceCurrentPtr& current) 
: _servant(servant)
, _current(current)
{
    ServantProxyCallback::setType(type);
}

int ServantCallback::onDispatch(ReqMessagePtr msg) 
{ 
    _servant->getResponseQueue().push_back(msg);

    _servant->getHandle()->notifyFilter();

    return 0;
}

const ServantPtr& ServantCallback::getServant()
{
    return _servant;
}
       
const JceCurrentPtr& ServantCallback::getCurrent()
{
    return _current;
}

///////////////////////////////////////////////////////////////////////////
RouterCallback::RouterCallback(const string& type, const ServantPtr& servant, const JceCurrentPtr& current) 
: ServantCallback(type,servant,current)
{
}

int RouterCallback::onDispatch(ReqMessagePtr msg) 
{ 
    _current->sendResponse(msg->response.iRet, msg->response.sBuffer,msg->response.status,msg->response.sResultDesc);

    return 0;
}

////////////////////////////////////////////////////////////////////////
}
