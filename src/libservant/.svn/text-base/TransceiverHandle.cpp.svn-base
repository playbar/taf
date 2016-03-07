#include "servant/TransceiverHandle.h"
#include "log/taf_logger.h"
#include "servant/FDReactor.h"

namespace taf
{

TransceiverHandle::TransceiverHandle(Communicator* comm)
: /*_bTerminate(false),*/ _comm(comm), _fdReactor(NULL)
{
}

TransceiverHandle::~TransceiverHandle()
{
//    terminate();
}

void TransceiverHandle::setFDReactor(FDReactor *fdReactor)
{
    _fdReactor = fdReactor;
}

void TransceiverHandle::registerProxy(int fd, TransceiverPtr& trans, const AdapterPrx& adapter)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    TransceiverHandle::ProxyInfo pi;

    pi.fd = fd;

    pi.trans = trans;

    pi.adapter = adapter;

    _proxyInfo[fd] = pi;
}

/*
void TransceiverHandle::terminate()
{
    _bTerminate = true;

    _eventQueue.notifyT();
}
*/

void TransceiverHandle::handle(int fd, int event)
{
    ProxyInfo pi;
    {
        TC_LockT<TC_ThreadMutex> lock(*this);

        map<int, TransceiverHandle::ProxyInfo>::iterator it = _proxyInfo.find(fd);

        if (it != _proxyInfo.end())
        {
            pi = it->second;
        }
    }

    if(pi.trans)
    {
        pi.event = event;

        if(pi.event & TransceiverHandle::R)
        {
            handleInputImp(pi);
        }

        if(pi.event & TransceiverHandle::W)
        {
            handleOutputImp(pi);
        }

        if (!pi.trans->isValid())
        {
            pi.adapter->refreshTransceiver();
        }

//        _eventQueue.push_back(pi);
    }
}

void TransceiverHandle::handleWrite(int fd)
{
    _fdReactor->notify(fd);
}

int TransceiverHandle::handleExcept(int fd)
{
    ProxyInfo pi;
    {
        TC_LockT<TC_ThreadMutex> lock(*this);

        map<int, TransceiverHandle::ProxyInfo>::iterator it = _proxyInfo.find(fd);

        if (it != _proxyInfo.end())
        {
            pi = it->second;

            _proxyInfo.erase(it);
        }
    }

    if(pi.trans)
    {
        pi.trans->doClose();

        pi.adapter->refreshTransceiver();
    }

	return 0;
}

void TransceiverHandle::handleInputImp(TransceiverHandle::ProxyInfo& pi)
{
    if (pi.trans->isValid())
    {
        list<ResponsePacket> done;

        if (pi.trans->doResponse(done) > 0)
        {
            for (list<ResponsePacket>::iterator lit = done.begin(); lit != done.end(); ++lit)
            {
                ReqMessagePtr req = new ReqMessage;

                req->response = *lit;

                pi.adapter->finished(req);
            }
        }
    }
}

void TransceiverHandle::handleOutputImp(TransceiverHandle::ProxyInfo& pi)
{
    if (pi.trans->isValid())
    {
        while (pi.trans->doRequest() >= 0 && pi.adapter->sendRequest(pi.trans));
    }
}
/*
void TransceiverHandle::run()
{
    while (!_bTerminate)
    {
        ProxyInfo pi;

        try
        {
            if (_eventQueue.pop_front(pi, -1))
            {
                if(pi.event & TransceiverHandle::R)
                {
                    handleInputImp(pi);
                }

                if(pi.event & TransceiverHandle::W)
                {
                    handleOutputImp(pi);
                }

                if (!pi.trans->isValid())
                {
                    pi.adapter->refreshTransceiver();
                }
            }
        }
        catch(exception &ex)
        {
            LOG->error() << "[TAF][TransceiverHandle::run ex:" << ex.what() << "]" << endl;
        }
        catch(...)
        {
            LOG->error() << "[TAF][TransceiverHandle::run ex.]" << endl;
        }
    }
}
*/
//////////////////////////////////////////////////////////////////////////////////////

}
