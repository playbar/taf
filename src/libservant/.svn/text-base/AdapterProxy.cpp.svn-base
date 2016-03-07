#include "servant/AdapterProxy.h"
#include "servant/StatReport.h"
#include "servant/Application.h"
#include "log/taf_logger.h"

namespace taf
{
AdapterProxy::AdapterProxy(Communicator* comm, 
                           const EndpointInfo &ep, 
                           ObjectProxy* op, 
                           FDReactor* rt, 
                           TransceiverHandle* transHandle)
: BaseProxy(comm)
, _endpoint(ep)
, _objectProxy(op)
, _reactor(rt)
, _transHandle(transHandle)
, _timeoutQueue(NULL)
, _checkTransInterval(10)
, _lastCheckTransTime(0)
, _currentGridGroup(0)
, _connectTimeout(1000)
, _timeoutInvoke(0)
, _totalInvoke(0)
, _frequenceFailInvoke(0)
, _lastFinishInvokeTime(TC_TimeProvider::getInstance()->getNow())
, _lastRetryTime(0)
, _activeStatus(true)
{
   _currentGridGroup = ep.grid();

   _connectTimeout = TC_Common::strto<int32_t>(_comm->getProperty("connect-timeout", "1000"));

   if (_connectTimeout < 1000)
   {
       _connectTimeout = 1000;
   }

   _timeoutQueue = new TC_TimeoutQueue<ReqMessagePtr>(_objectProxy->timeout()); 

   _comm->getTimeoutHandle()->registerHandle(_timeoutQueue);
}

AdapterProxy::~AdapterProxy()
{
    _trans.clear();

    if (_timeoutQueue != NULL && _comm->getTimeoutHandle())
    {
        _comm->getTimeoutHandle()->unregisterHandle(_timeoutQueue);
    }

    delete _timeoutQueue;

    _timeoutQueue = NULL;
}

const EndpointInfo& AdapterProxy::endpoint()
{
    return _endpoint;
}

void AdapterProxy::setActive(bool value)
{
    _activeStatus = value;
}

TransceiverPtr AdapterProxy::doReconnect()
{
    try
    {
        TransceiverPtr t = Connector::connect(_objectProxy, _endpoint, _connectTimeout);

        LOG->info() << "[TAF][doReconnect," << _objectProxy->name() << ",connect succ:" << _endpoint.desc() << ",fd:" << t->fd() << "]" << endl;

        _transHandle->registerProxy(t->fd(), t, this);

        _reactor->registerHandle(t->fd(), EPOLLIN|EPOLLOUT, _transHandle);

        _trans.push_back(t);

        return t;
    }
    catch (TafException& e)
    {
        LOG->error() << "[TAF][doReconnect," << _objectProxy->name() << ",connect fail:" << _endpoint.desc() << ",ex:" << e.what() << "]" << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][doReconnect," << _objectProxy->name() << ",connect fail,ep:" << _endpoint.desc() << "]" << endl;
    }
    return NULL;
}

/**
 * Adapter自己保存的有路由状态的消息
 */
bool AdapterProxy::popRequest(ReqMessagePtr& req)
{
    if (_timeoutQueue)
    {
        ReqMessagePtr ptr = _timeoutQueue->pop();

        if (ptr)
        {
            req = ptr;

            return true;
        }
    }
    return false;
}

/**
 * 由Transceiver响应了EpollOut后调用，获取待发送的消息
 */
bool AdapterProxy::sendRequest(TransceiverPtr& trans)
{
    int len =  0;

    ReqMessagePtr req;

    //Adapter自己保存的有状态的消息优先
    while (this->popRequest(req) || _objectProxy->popRequest(req))
    {       
        req->endpoint = _endpoint;

        //真正发送数据的adapter
        req->adapter  = this;

        string s = "";

        _objectProxy->getProxyProtocol().requestFunc(req->request, s);

        //同步调用，保存最后一个请求
        //当连接出现异常时，可以及时唤醒等待的业务线程
        if (req->type == ReqMessage::SYNC_CALL)
        {
            trans->setLastReq(req);
        }

        trans->writeToSendBuffer(s);

        LOG->info() << "[TAF][sendRequest, " << _objectProxy->name() << ", " << _endpoint.desc() << ", id:" << req->request.iRequestId << ", len:" << s.length() << "]" << endl;
        
        len += s.length();
        //合并一次发送的包 最大合并至8k 提高异步时客户端效率
        if (trans->getEndpointType() == EndpointInfo::UDP || len > 8192)
        {
            break;
        }
    }
    return len>0;
}

int AdapterProxy::invoke(ReqMessagePtr& req)
{
    //选择一个连接
    TransceiverPtr trans = selectTransceiver();

    if (!trans)
    {
        LOG->error() << "[TAF][invoke," << _objectProxy->name() << ",select trans and retry fail,ep:" << _endpoint.desc() << "]" << endl;

        return -1;
    }
    
    if (req->fromRpc == false)
    {
        req->request.iRequestId = _objectProxy->getTimeoutQueue()->generateId();
    }

    //1.如果没有路由状态，则放入ObjectProxy队列中
    //2.如果是HASH或者路由状态，则放入Adapter自己的队列中
    //3.注意，使用的都是ObjectProxy的TimeoutQueue产生的id
    if (IS_MSG_TYPE(req->request.iMessageType, taf::JCEMESSAGETYPEHASH)
        || IS_MSG_TYPE(req->request.iMessageType, taf::JCEMESSAGETYPEGRID))
    {
        _timeoutQueue->push(req, req->request.iRequestId);
    }
    else
    {
        _objectProxy->getTimeoutQueue()->push(req, req->request.iRequestId);
    }

    //虽然这个地方选出了连接，但是实际上不一定是这个连接发送的数据
   // _transHandle->handle(trans->fd(), TransceiverHandle::W);
    _transHandle->handleWrite(trans->fd());

    //采样统计
    sampleToStat(req, _objectProxy->name(), _endpoint.host());

    return 0;
}

bool AdapterProxy::dispatchPushCallback(ReqMessagePtr& req)
{
    ServantProxyCallback* cb = _objectProxy->getOneRandomPushCallback();

    if (cb)
    {
        req->adapter = this;

        cb->onDispatch(req);

        return true;
    }
    return false;
}

int AdapterProxy::finished(ReqMessagePtr& req)
{
    //如果灰度路由选取的服务端状态不匹配，以服务端为准
    //reset,将Adapter从现在group中转移到服务端返回的group中
    //不修改本地Adapter::Endpoint的grid属性
    if (req->response.iRet == JCESERVERRESETGRID)
    {
        const map<string, string>& respStatus = req->response.status;

        map<string, string>::const_iterator sit = respStatus.find(ServantProxy::STATUS_GRID_CODE);

        if (sit != respStatus.end())
        {
            int32_t newGrid = TC_Common::strto<int32_t>(sit->second);

            LOG->info() << "[TAF][finished," << _objectProxy->name() << ",server reset grid msg:" << _currentGridGroup << "->" << newGrid << "]" << endl;

            if (_objectProxy->resetAdapterGrid(_currentGridGroup, newGrid, this))
            {
                _currentGridGroup = newGrid;
            }
        }
    }

    //获取请求时的id和对应的request包
    uint32_t id = req->response.iRequestId;

    ReqMessagePtr ptr = NULL;

    //1.如果有路由状态，则从AdapterProxy自己的队列中查找request
    //2.如果没有路由状态，则从ObjectProxy队列中查找request
    if (IS_MSG_TYPE(req->response.iMessageType, taf::JCEMESSAGETYPEHASH)
        || IS_MSG_TYPE(req->response.iMessageType, taf::JCEMESSAGETYPEGRID))
    {
        if (!_timeoutQueue)
        {
            LOG->error() << "[TAF][finished," << _objectProxy->name() << ",adapter proxy's timeoutQueue is NULL]" << endl;

            return -1;
        }
        ptr = _timeoutQueue->get(id);
    }
    else
    {
        ptr = _objectProxy->getTimeoutQueue()->get(id);

        //如果是第三方协议服务的响应，会丢失HASH状态
        //所以要尝试从AdpaterProxy的队列中再get一次
        if (!ptr && _timeoutQueue)
        {
            ptr = _timeoutQueue->get(id);
        }
    }

    if (!ptr)
    {
        if (!dispatchPushCallback(req))
        {
            LOG->error() << "[TAF][finished," << _objectProxy->name() << ",get req-ptr from timeoutqueue ret NULL,may be timeout,id:" << id << "]" << endl;
        }
        return -1;
    }

    ptr->response = req->response;

    //同步调用，唤醒ServantProxy线程
    if (ptr->type == ReqMessage::SYNC_CALL)
    {
        return ptr->proxy->finished(ptr);
    }

    //异步回调，放入回调处理线程中
    if (ptr->callback)
    {
        _comm->asyncProcThread()->put(ptr);

        return 0;
    }

    LOG->error() << "[TAF][finished," << _objectProxy->name() << ",adapter proxy finish fail,id:" << id << ",ret:" << req->response.iRet << "]" << endl;

    return -1;
}

void AdapterProxy::refreshTransceiver()
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    //刷新所有的连接状态，删除无效的连接
    //由TranscieverHandle收到网络异常时调用
    vector<TransceiverPtr>::iterator it = _trans.begin();

    while (it != _trans.end())
    {
        (*it)->isValid() ? ++it : _trans.erase(it);
    }
}

TransceiverPtr AdapterProxy::selectTransceiver()
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    //随机选取一个连接, 如果某个连接不可用，则从vector中删除
    if (!_trans.empty())
    {
        uint32_t rnum = rand() % _trans.size();

        vector<TransceiverPtr>::iterator it = _trans.begin() + rnum;

        while (it != _trans.end())
        {
            if ((*it)->isValid()) { return *it; }

            _trans.erase(it);
        }

        it = _trans.begin();

        while (rnum-- >= 0 && it != _trans.end())
        {
            if ((*it)->isValid()) { return *it; }

            _trans.erase(it);
        }
    }
    return doReconnect();
}

void AdapterProxy::finishInvoke(bool bTimeout)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    time_t now = TC_TimeProvider::getInstance()->getNow();

    _frequenceFailInvoke = (bTimeout? (_frequenceFailInvoke + 1) : 0);
        
    //处于异常状态且重试成功,恢复正常状态
    if (!_activeStatus && !bTimeout)
    {
        _activeStatus = true;

        _lastFinishInvokeTime = now;

        _frequenceFailInvoke = 0;

        _totalInvoke = 1;

        _timeoutInvoke = 0;

        //导致死锁 屏蔽掉
        //TAF_NOTIFY_ERROR(_endpoint.desc() + ": enabled");

        LOG->info() << "[TAF][finishInvoke, " << _objectProxy->name() << ", " << _endpoint.desc() << ", retry ok]" << endl;

        return;
    }

    //处于异常状态且重试失败
    if (!_activeStatus && (now - _lastFinishInvokeTime >= _lastRetryTime))
    {
        LOG->info() << "[TAF][finishInvoke, " << _objectProxy->name() << ", " << _endpoint.desc() << ", retry fail]" << endl;

        return;
    }

    ++_totalInvoke;

    if (bTimeout) 
    { 
        ++_timeoutInvoke; 
    }

    CheckTimeoutInfo& info = _objectProxy->checkTimeoutInfo();

    //判断是否进入到下一个计算时间片  
    uint32_t interval = uint32_t(now - _lastFinishInvokeTime);
    if (interval >= info.checkTimeoutInterval || (_frequenceFailInvoke >= info.frequenceFailInvoke && interval >10)) //至少大于1秒
    {
        _lastFinishInvokeTime = now;

        //计算是否超时比例超过限制了
        if (_timeoutInvoke >= info.minTimeoutInvoke && ((1.0*_timeoutInvoke/_totalInvoke) >= info.radio))
        {
            _activeStatus = false;

            LOG->error() << "[TAF][finishInvoke, " 
                         << _objectProxy->name() << "," << _endpoint.desc() 
                         << ",disable,freqtimeout:" << _frequenceFailInvoke 
                         << ",timeout:"<< _timeoutInvoke 
                         << ",total:" << _totalInvoke << "] " << endl;

            //导致死锁 屏蔽掉
            //TAF_NOTIFY_ERROR(_endpoint.desc() + ": disabled because of too many timeout.");

            resetInvoke();
        }
        else
        {
            _frequenceFailInvoke = 0;

            _totalInvoke = 0;

            _timeoutInvoke = 0;
        }
    }
}

bool AdapterProxy::checkActive()
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    time_t now = TC_TimeProvider::getInstance()->getNow();

    LOG->info() << "[TAF][checkActive," 
                << _objectProxy->name() << "," << _endpoint.desc() << "," 
                << (_activeStatus ? "enable" : "disable") 
                << ",freqtimeout:" << _frequenceFailInvoke 
                << ",timeout:" << _timeoutInvoke 
                << ",total:" << _totalInvoke << "]" << endl;

    //失效且没有到下次重试时间, 直接返回不可用
    if(!_activeStatus && (now - _lastRetryTime < (int)_objectProxy->checkTimeoutInfo().tryTimeInterval))
    {
        return false;
    }

    _lastRetryTime = now;

    //检查是否需要建立新的连接
    if (_trans.size() < (size_t)_objectProxy->getAdapterMaxTransNum() 
        && (_lastCheckTransTime + _checkTransInterval < now))
    {
        _lastCheckTransTime = now;

        doReconnect();
    }

    return (_trans.size() > 0);
}

void AdapterProxy::resetInvoke()
{
    _lastFinishInvokeTime = TC_TimeProvider::getInstance()->getNow();

    _lastRetryTime = TC_TimeProvider::getInstance()->getNow();

    _frequenceFailInvoke = 0;

    _totalInvoke = 0;

    _timeoutInvoke = 0;

    //连接全部清除
    vector<TransceiverPtr>::iterator it = _trans.begin();

    while (it != _trans.end())
    {
         (*it)->doClose();

         _trans.erase(it);
    }
}
//////////////////////////////////////////////////////////////////////////////////
}
