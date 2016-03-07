#include "tc_http_async.h"

namespace taf
{

TC_HttpAsync::AsyncRequest::AsyncRequest(TC_HttpRequest &stHttpRequest, TC_HttpAsync::RequestCallbackPtr &callbackPtr) 
    : _pHttpAsync(NULL), _callbackPtr(callbackPtr)
{
    _sReq = stHttpRequest.encode();
    
    stHttpRequest.getHostPort(_sHost, _iPort);
}

TC_HttpAsync::AsyncRequest::~AsyncRequest()
{
    doClose();
}

void TC_HttpAsync::AsyncRequest::doClose()
{
    if(_fd.isValid())
    {
        _callbackPtr->onClose();
        if(_pHttpAsync) _pHttpAsync->erase(_iUniqId);
        _fd.close();
    }
}

int TC_HttpAsync::AsyncRequest::doConnect()
{
    _fd.createSocket();
    _fd.setblock();

    try
    {
        _fd.connect(_sHost, _iPort);
    }
    catch(TC_SocketConnect_Exception &ex)
    {
        if(errno != EINPROGRESS)
        {
            _fd.close();
            return -1;
        }
    }
    catch(exception &ex)
    {
        return -2;
    }

    return 0;
}
       
int TC_HttpAsync::AsyncRequest::recv(void* buf, uint32_t len, uint32_t flag)
{
    int ret = ::recv(_fd.getfd(), buf, len, flag);

    if (ret == 0)
    {
        return 0;
    }
    else if(ret < 0 && errno == EAGAIN)
    {
        return -1;
    }
    else if(ret < 0)
    {
        //其他网络异常
        return -2;
    }

    //正常发送的数据
    return ret;
}
        
int TC_HttpAsync::AsyncRequest::send(const void* buf, uint32_t len, uint32_t flag)
{
    int ret = ::send(_fd.getfd(), buf, len, flag);

    if (ret < 0 && errno == EAGAIN)
    {
        return -1;
    }
    else if (ret < 0)
    {
        return -2;
    }
    return ret;
}        

void TC_HttpAsync::AsyncRequest::timeout()
{
    try
    {
        doClose();

        _callbackPtr->onTimeout();
    }
    catch(exception &ex)
    {
        _callbackPtr->onException(ex.what());
    }
    catch(...)
    {
        _callbackPtr->onException("unknown error.");
    }
}

void TC_HttpAsync::AsyncRequest::doException()
{
    string err("unknown error.");

    if(_fd.isValid())
    {
        int error;
        socklen_t len = sizeof(error);
        _fd.getSockOpt(SO_ERROR, (void*)&error, len, SOL_SOCKET);
        err = strerror(error);
        doClose();
    }
    if(comment.size())comment+=";";
    
    try { _callbackPtr->onException(comment+string(err)); } catch(...) { }
}

void TC_HttpAsync::AsyncRequest::doRequest()
{
    if(!_fd.isValid()) return;

    int ret = -1;

    do
    {
        ret = -1;
        
        if (!_sReq.empty())
        {

            if ((ret = this->send(_sReq.c_str(), _sReq.length(), 0)) > 0)
            {
                _sReq = _sReq.substr(ret);
            }
        }
    }while(ret > 0);

	//网络异常
	if(ret == -2)
	{
		doException();
	}
}

void TC_HttpAsync::AsyncRequest::doReceive()
{
    if(!_fd.isValid()) return;

    int recv = 0;

    char *buff = new char[8192];
    do
    {

        if ((recv = this->recv(buff, sizeof(buff), 0)) > 0)
        {
            _sRsp.append(buff, recv);
        }
    }
    while (recv > 0);

    delete []buff;

    if(recv == -2)
    {
        doException();
    }
    else
    {
        try
        {
            //增量decode
            bool ret    = _stHttpResp.incrementDecode(_sRsp);

            //服务器关闭了连接
            bool bClose = (recv == 0);

            //如果远程关闭连接或者增量decode完毕
            if(bClose || ret)
            {
                doClose();

                _callbackPtr->onResponse(bClose, _stHttpResp);
            }
        }
        catch(exception &ex)
        {
            _callbackPtr->onException(ex.what());
        }
        catch(...)
        {
            _callbackPtr->onException("unknown error.");
        }
    }
}

///////////////////////////////////////////////////////////////////////////

TC_HttpAsync::TC_HttpAsync() : _terminate(false)
{
    _data = new http_queue_type(10000);

    _epoller.create(1024);
}

TC_HttpAsync::~TC_HttpAsync()
{
    terminate();

    delete _data;

    for(size_t i = 0; i < _npool.size(); i++)
    {
        delete _npool[i];
    }
    _npool.clear();
}

void TC_HttpAsync::start(int iThreadNum)
{
    if(_npool.size() > 0)
        throw TC_HttpAsync_Exception("[TC_HttpAsync::start] thread has started.");

    _tpool.init(1);
    _tpool.start();

    if(iThreadNum <= 0) iThreadNum = 1;

    for(int i = 0; i < iThreadNum; i++)
    {
        _npool.push_back(new TC_ThreadPool());
        _npool.back()->init(1);
        _npool.back()->start();
    }

    TC_Functor<void> cmd(this, &TC_HttpAsync::run);
    TC_Functor<void>::wrapper_type wt(cmd);

    _tpool.exec(wt);
}

void TC_HttpAsync::waitForAllDone(int millsecond)
{
    time_t now = TC_TimeProvider::getInstance()->getNow();

    while(_data->size() > 0)
    {
        if(millsecond < 0)
        {
            TC_ThreadLock::Lock lock(*this);
            timedWait(100);
            continue;
        }

        {
            //等待100ms
            TC_ThreadLock::Lock lock(*this);
            timedWait(100);
        }

        if((TC_TimeProvider::getInstance()->getNow() - now) >= (millsecond/1000))
            break;
    }

    terminate();
}

void TC_HttpAsync::erase(uint32_t uniqId) 
{ 
    _data->erase(uniqId); 

    TC_ThreadLock::Lock lock(*this);
    notify();
}

void TC_HttpAsync::terminate()
{
    _terminate = true;

    for(size_t i = 0; i < _npool.size(); i++)
    {
        _npool[i]->waitForAllDone();
    }

    _tpool.waitForAllDone();
}

void TC_HttpAsync::timeout(AsyncRequestPtr& ptr) 
{ 
    ptr->timeout(); 
}

int TC_HttpAsync::doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr)
{
    AsyncRequestPtr req = new AsyncRequest(stHttpRequest, callbackPtr);

    //发起异步连接请求
    int ret = req->doConnect();

    if(ret < 0) return -1;

    uint32_t uniqId = _data->generateId();

    req->setUniqId(uniqId);

    req->setHttpAsync(this);

    _data->push(req, uniqId);

    _epoller.add(req->getfd(), uniqId, EPOLLIN | EPOLLOUT);

    return 0;
}

void TC_HttpAsync::process(AsyncRequestPtr p, int events)
{
    if (events & (EPOLLERR | EPOLLHUP))
    {
        p->doException();
        return;
    }

    if(events & EPOLLIN)
    {
        p->doReceive();
    }

    if(events & EPOLLOUT)
    {
        p->doRequest();
    } 
}

void TC_HttpAsync::run() 
{
    TC_TimeoutQueue<AsyncRequestPtr>::data_functor df(&TC_HttpAsync::timeout);
    
    async_process_type apt(&TC_HttpAsync::process);

    while(!_terminate)
    {
        try
        {
            _data->timeout(df);
            
            int num = _epoller.wait(1000);

            for (int i = 0; i < num; ++i)
            {
                epoll_event ev = _epoller.get(i);

                uint32_t uniqId = (uint32_t)ev.data.u64;

                AsyncRequestPtr p = _data->get(uniqId, false);
                
                if(!p) continue;

                async_process_type::wrapper_type w(apt, p, ev.events);

                _npool[uniqId%_npool.size()]->exec(w);
            }
        }
        catch(exception &ex)
        {
            cerr << "[TC_HttpAsync::run] error:" << ex.what() << endl;
        }
    }
}

}


