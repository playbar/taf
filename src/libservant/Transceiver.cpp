#include "servant/Transceiver.h"
#include "log/taf_logger.h"

namespace taf
{
///////////////////////////////////////////////////////////////////////
Transceiver::Transceiver(ObjectProxy *op, EndpointInfo ep, int fd)
: _fd(fd)
, _ep(ep)
, _objectProxy(op)
{
    //预分配好一定的内存
    _sendBuffer.reserve(1024);
}

Transceiver::~Transceiver()
{
    doClose();
}

void Transceiver::close()
{
    doClose();

    if (_lastReq && _lastReq->monitor)
    {
        _lastReq->response.iRet = JCEPROXYCONNECTERR;

        TC_ThreadLock::Lock sync(*(_lastReq->monitor));

        _lastReq->monitor->notify();
    }
}

void Transceiver::doClose()
{
    if(!isValid()) return;

    NetworkUtil::closeSocketNoThrow(_fd);

    _fd = -1;

    LOG->info() << "[TAF][trans close:" << _objectProxy->name() << "," << _ep.desc() << "]" << endl;
}

int Transceiver::fd() const
{
    return _fd;
}

bool Transceiver::isValid() const
{
    return (_fd != -1);
}

void Transceiver::setLastReq(ReqMessagePtr& req)
{
    _lastReq = req;
}

int Transceiver::doRequest()
{
    if(!isValid()) return -1;

    int ret = 0;

    do
    {
        ret = 0;

        if (!_sendBuffer.empty())
        {
            size_t length = _sendBuffer.length();

            ret = this->send(_sendBuffer.c_str(), _sendBuffer.length(), 0);
            if(ret > 0)
            {
                if((size_t)ret == length)
                {
                    _sendBuffer.clear();
                }
                else
                {
                    _sendBuffer.erase(_sendBuffer.begin(), _sendBuffer.begin() + min((size_t)ret, length));
                }
            }
        }
    }
    while (ret > 0);

    return ret;
}

void Transceiver::writeToSendBuffer(const string& msg)
{
    _sendBuffer.append(msg);
}

//////////////////////////////////////////////////////////
TcpTransceiver::TcpTransceiver(ObjectProxy *op, EndpointInfo ep, int fd)
: Transceiver(op, ep, fd)
{
    //预分配好一定的内存
    _recvBuffer.reserve(1024);
}

int TcpTransceiver::doResponse(list<ResponsePacket>& done)
{
    if(!isValid()) return -1;

    int recv = 0;

    done.clear();

    char buff[8192] = {0};

    do
    {
        if ((recv = this->recv(buff, sizeof(buff), 0)) > 0)
        {
            _recvBuffer.append(buff, recv);
        }
    }
    while (recv > 0);

    LOG->info() << "[TAF][tcp doResponse, " << _objectProxy->name() << ",fd:" << _fd << ",recvbuf:" << _recvBuffer.length() << "]" << endl;

    if(!_recvBuffer.empty())
    {
        try
        {
            size_t pos = _objectProxy->getProxyProtocol().responseFunc(_recvBuffer.c_str(), _recvBuffer.length(), done);

            if(pos > 0)
            {
                //用erase, 不用substr, 从而可以保留预分配的空间
                _recvBuffer.erase(_recvBuffer.begin(), _recvBuffer.begin() + min(pos, _recvBuffer.length()));
                
                if(_recvBuffer.capacity() - _recvBuffer.length() > 102400)
                {
                   _recvBuffer.reserve(max(_recvBuffer.length(),(size_t)1024));
                }
            }
        }
        catch (exception &ex)
        {
            LOG->error() << "[TAF][tcp doResponse," << _objectProxy->name() << ",fd:" << _fd << "," << _ep.desc() << ",tcp recv decode error:" << ex.what() << endl;

            close();
        }
        catch (...)
        {
            LOG->error() << "[TAF][tcp doResponse," << _objectProxy->name() << ",fd:" << _fd << "," << _ep.desc() << ",tcp recv decode error." << endl;

            close();
        }
    }
    return done.empty()?0:1;
}

int TcpTransceiver::send(const void* buf, uint32_t len, uint32_t flag)
{
    if(!isValid()) return -1;

    int ret = ::send(_fd, buf, len, flag);

    if (ret < 0 && errno != EAGAIN)
    {
        LOG->info() << "[TAF][tcp send," << _objectProxy->name() << ",fd:" << _fd << "," << _ep.desc() << ",fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl;

        close();

        return 0;
    }
    LOG->info() << "[TAF][tcp send," << _objectProxy->name() << ",fd:" << _fd << "," << _ep.desc() << ",len:" << len << "]" << endl;

    return ret;
}

int TcpTransceiver::recv(void* buf, uint32_t len, uint32_t flag)
{
    if(!isValid()) return -1;

    int ret = ::recv(_fd, buf, len, flag);

    if (ret == 0 || (ret < 0 && errno != EAGAIN))
    {
        LOG->info() << "[TAF][tcp recv, " << _objectProxy->name() << ",fd:" << _fd << ", " << _ep.desc() << ", fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl;

        close();

        return 0;
    }
    LOG->info() << "[TAF][tcp recv," << _objectProxy->name() << ",fd:" << _fd << "," << _ep.desc() << ",ret:" << ret << "]" << endl;

    return ret;
}

/////////////////////////////////////////////////////////////////
UdpTransceiver::UdpTransceiver(ObjectProxy *op, EndpointInfo ep, int fd)
: Transceiver(op, ep, fd)
{
}

int UdpTransceiver::doResponse(list<ResponsePacket>& done)
{
    if(!isValid()) return -1;

    int recv = 0;

    done.clear();

    char buff[8192] = {0};

    do
    {
        if ((recv = this->recv(buff, sizeof(buff), 0)) > 0)
        {
            LOG->info() << "[TAF][udp doResponse, " << _objectProxy->name() << ",fd:" << _fd << ",recvbuf:" << recv << "]" << endl;

            try
            {
                _objectProxy->getProxyProtocol().responseFunc(buff, recv, done);
            }
            catch (exception &ex)
            {
                LOG->error() << "[TAF][udp doResponse, " << _objectProxy->name() << ",fd:" << _fd<< "," << _ep.desc() << ", udp recv decode error:" << ex.what() << endl;
            }
            catch (...)
            {
                LOG->error() << "[TAF][udp doResponse, " << _objectProxy->name() << ",fd:" << _fd<< "," << _ep.desc() << ", udp recv decode error." << endl;
            }
        }
    }
    while (recv > 0);

    return done.empty()?0:1;
}

int UdpTransceiver::send(const void* buf, uint32_t len, uint32_t flag)
{
    if(!isValid()) return -1;

    struct sockaddr addr;

    ::memcpy(&addr, &_ep.addr(), sizeof(struct sockaddr));

    int ret = ::sendto(_fd, buf, len, flag, &addr, sizeof(addr));

    if (ret < 0 && errno != EAGAIN)
    {
        LOG->info() << "[TAF][udp send " << _objectProxy->name() << ",fd:" << _fd << "," << _ep.desc() << ", fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl;

        close();

        return 0;
    }
    return ret;
}

int UdpTransceiver::recv(void* buf, uint32_t len, uint32_t flag)
{
    if(!isValid()) return -1;

    int ret = ::recvfrom(_fd, buf, len, flag, NULL, NULL); //need check from_ip & port

    if (ret < 0 && errno != EAGAIN)
    {
        LOG->info() << "[TAF][udp recv " << _objectProxy->name() << ",fd:" << _fd << "," << _ep.desc() << ", fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl;

        close();

        return 0;
    }
    return ret;
}
/////////////////////////////////////////////////////////////////
}
