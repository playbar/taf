#include "servant/NetworkUtil.h"
#include "servant/Global.h"
#include "util/tc_epoller.h"

#include <sys/epoll.h>

/*********************************************
 * Source From ICE:)
*********************************************/
#if defined(_WIN32)
#  include <winsock2.h>
#elif defined(__linux) || defined(__APPLE__) || defined(__FreeBSD__)
#  include <ifaddrs.h>
#else
#  include <sys/ioctl.h>
#  include <net/if.h>
#  ifdef __sun
#    include <sys/sockio.h>
#  endif
#endif

#include <sstream>
#include <assert.h>

using namespace std;
using namespace taf;

string NetworkUtil::inetAddrToString(const struct in_addr& in)
{
    char sAddr[INET_ADDRSTRLEN] = "\0";

    inet_ntop(AF_INET, &in, sAddr, sizeof(sAddr));

    return sAddr;
}

int NetworkUtil::getSocketErrno()
{
    return errno;
}

bool NetworkUtil::interrupted()
{
    return errno == EINTR;
}

bool NetworkUtil::acceptInterrupted()
{
    if(interrupted())
    {
        return true;
    }
    return errno == ECONNABORTED || errno == ECONNRESET || errno == ETIMEDOUT;
}

bool NetworkUtil::noBuffers()
{
    return errno == ENOBUFS;
}

bool NetworkUtil::wouldBlock()
{
    return errno == EAGAIN || errno == EWOULDBLOCK;
}

bool NetworkUtil::connectFailed()
{
    return errno == ECONNREFUSED ||
           errno == ETIMEDOUT ||
           errno == ENETUNREACH ||
           errno == EHOSTUNREACH ||
           errno == ECONNRESET ||
           errno == ESHUTDOWN ||
           errno == ECONNABORTED;
}

bool NetworkUtil::connectionRefused()
{
    return errno == ECONNREFUSED;
}

bool NetworkUtil::connectInProgress()
{
    return errno == EINPROGRESS;
}

bool NetworkUtil::connectionLost()
{
    return  errno == ECONNRESET ||
            errno == ENOTCONN ||
            errno == ESHUTDOWN ||
            errno == ECONNABORTED ||
            errno == EPIPE;
}

bool NetworkUtil::notConnected()
{
    return errno == ENOTCONN;
}

bool NetworkUtil::recvTruncated()
{
    return false;
}

int NetworkUtil::createSocket(bool udp, bool isLocal/* = false*/)
{
    int fd;

    if (udp)
    {
        fd = socket((isLocal ? PF_LOCAL : PF_INET), SOCK_DGRAM, IPPROTO_UDP);
    }
    else
    {
        fd = socket((isLocal ? PF_LOCAL : PF_INET), SOCK_STREAM, IPPROTO_TCP);
    }

    if (fd == INVALID_SOCKET)
    {
        ostringstream os;
        os << "createSocket ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }

    if(!udp)
    {
        setTcpNoDelay(fd);

        setKeepAlive(fd);
    }

    return fd;
}

void NetworkUtil::closeSocket(int fd)
{
    int error = errno;

    if(close(fd) == SOCKET_ERROR)
    {
        ostringstream os;
        os << "closeSocket ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
    errno = error;
}

void NetworkUtil::closeSocketNoThrow(int fd)
{
    int error = errno;
    close(fd);
    errno = error;
}

void NetworkUtil::shutdownSocketWrite(int fd)
{
    if (shutdown(fd, SHUT_WR) == SOCKET_ERROR)
    {
        if(errno == ENOTCONN)
        {
            return;
        }
    }
}

void NetworkUtil::shutdownSocketReadWrite(int fd)
{
    if (shutdown(fd, SHUT_RDWR) == SOCKET_ERROR)
    {
        if(errno == ENOTCONN)
        {
            return;
        }
    }
}

void NetworkUtil::setBlock(int fd, bool block)
{
    if (block)
    {
        int flags = fcntl(fd, F_GETFL);
        flags &= ~O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == SOCKET_ERROR)
        {
            closeSocketNoThrow(fd);
            ostringstream os;
            os << "setBlock ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TafNetSocketException(os.str());
        }
    }
    else
    {
        int flags = fcntl(fd, F_GETFL);
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == SOCKET_ERROR)
        {
            closeSocketNoThrow(fd);
            ostringstream os;
            os << "setBlock ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TafNetSocketException(os.str());
        }
    }
}

void NetworkUtil::setTcpNoDelay(int fd)
{
    int flag = 1;
    if(setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "setTcpNoDelay ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
}

void NetworkUtil::setKeepAlive(int fd)
{
    int flag = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "setKeepAlive ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
}

void NetworkUtil::setSendBufferSize(int fd, int sz)
{
    if(setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&sz, int(sizeof(int))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "setSendBufferSize ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
}

int NetworkUtil::getSendBufferSize(int fd)
{
    int sz;
    socklen_t len = sizeof(sz);
    if(getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&sz, &len) == SOCKET_ERROR || len != sizeof(sz))
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "getSendBufferSize ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
    return sz;
}

void NetworkUtil::setRecvBufferSize(int fd, int sz)
{
    if(setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&sz, int(sizeof(int))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "setRecvBufferSize ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
}

int NetworkUtil::getRecvBufferSize(int fd)
{
    int sz;
    socklen_t len = sizeof(sz);
    if(getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&sz, &len) == SOCKET_ERROR || len != sizeof(sz))
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "getRecvBufferSize ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
    return sz;
}

void NetworkUtil::doBind(int fd, struct sockaddr_in& addr)
{
    if(bind(fd, reinterpret_cast<struct sockaddr*>(&addr), int(sizeof(addr))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "doBind ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
    socklen_t len = static_cast<socklen_t>(sizeof(addr));
    getsockname(fd, reinterpret_cast<struct sockaddr*>(&addr), &len);
}

void NetworkUtil::doListen(int fd, int backlog)
{
repeatListen:
    if(::listen(fd, backlog) == SOCKET_ERROR)
    {
        if(interrupted())
        {
            goto repeatListen;
        }
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "doListen ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
}

void NetworkUtil::doConnect(int fd, struct sockaddr_in& addr, int timeout)
{
repeatConnect:
    if (::connect(fd, reinterpret_cast<struct sockaddr*>(&addr), int(sizeof(addr))) == SOCKET_ERROR)
    {
        if(interrupted())
        {
            goto repeatConnect;
        }

    	if(connectInProgress())
    	{
    	    int val;
repeatSelect:
	        TC_Epoller epoller(false);
            epoller.create(1);
            epoller.add(fd, 0, EPOLLOUT);
            int ret = epoller.wait(timeout);

    	    if(ret == 0)
    	    {
                closeSocketNoThrow(fd);
                ostringstream os;
                os << "ConnectTimeoutException:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
                throw TafNetSocketException(os.str());
    	    }
    	    else if(ret == SOCKET_ERROR)
    	    {
                if(interrupted())
                {
                    goto repeatSelect;
                }
                ostringstream os;
                os << "::select ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
                throw TafNetSocketException(os.str());
    	    }

    	    socklen_t len = static_cast<socklen_t>(sizeof(int));
    	    if(getsockopt(fd, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&val), &len) == SOCKET_ERROR)
    	    {
                closeSocketNoThrow(fd);
                ostringstream os;
                os << "getsockopt ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
                throw TafNetSocketException(os.str());
    	    }
    	    if(val > 0)
    	    {
                closeSocketNoThrow(fd);
                errno = val;
                if(connectionRefused())
                {
                    ostringstream os;
                    os << "connectionRefused ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
                    throw TafNetSocketException(os.str());
                }
                else if(connectionLost())
                {
                    ostringstream os;
                    os << "connectionLost ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
                    throw TafNetConnectLostException(os.str());
                }
                else if(connectFailed())
                {
                    ostringstream os;
                    os << "connectFailed ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
                    throw TafNetConnectException(os.str());
                }
                else
                {
                    ostringstream os;
                    os << "doConnect ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
                    throw TafNetConnectException(os.str());
                }
    	    }
    	    return;
    	}

        closeSocketNoThrow(fd);

        if(connectionRefused())
        {
            ostringstream os;
            os << "connectionRefused ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TafNetSocketException(os.str());
        }
        else if(connectionLost())
        {
            ostringstream os;
            os << "connectionLost ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TafNetConnectLostException(os.str());
        }
        else if(connectFailed())
        {
            ostringstream os;
            os << "connectFailed ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TafNetConnectException(os.str());
        }
        else
        {
            ostringstream os;
            os << "doConnect ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TafNetConnectException(os.str());
        }
    }
}

void NetworkUtil::getAddress(const string& host, int port, struct sockaddr_in& addr)
{
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());

    if(addr.sin_addr.s_addr == INADDR_NONE)
    {
    	struct addrinfo* info = 0;
    	int retry = 5;

    	struct addrinfo hints = { 0 };
    	hints.ai_family = PF_INET;

    	int rs = 0;
    	do
    	{
    	    rs = getaddrinfo(host.c_str(), 0, &hints, &info);
    	}
    	while(info == 0 && rs == EAI_AGAIN && --retry >= 0);

    	if(rs != 0)
    	{
            ostringstream os;
            os << "DNSException ex:(" << errorToString(errno) << ")" << rs << ":" << host << ":" << __FILE__ << ":" << __LINE__;
            throw TafNetSocketException(os.str());
    	}

    	assert(info->ai_family == PF_INET);
    	struct sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(info->ai_addr);

    	addr.sin_addr.s_addr = sin->sin_addr.s_addr;
    	freeaddrinfo(info);
    }
}

string NetworkUtil::errorToString(int error)
{
    return strerror(error);
}

string NetworkUtil::lastErrorToString()
{
    return errorToString(errno);
}

std::string NetworkUtil::fdToString(int fd)
{
    if(fd == INVALID_SOCKET)
    {
        return "<closed>";
    }
    struct sockaddr_in localAddr;
    fdToLocalAddress(fd, localAddr);

    struct sockaddr_in remoteAddr;
    bool peerConnected = fdToRemoteAddress(fd, remoteAddr);

    ostringstream s;
    s << "local address = " << addrToString(localAddr);
    if(peerConnected)
    {
        s << "\nremote address = " << addrToString(remoteAddr);
    }
    else
    {
        s << "\nremote address = <not connected>";
    }
    return s.str();
}

void NetworkUtil::fdToLocalAddress(int fd, struct sockaddr_in& addr)
{
    socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
    if(getsockname(fd, reinterpret_cast<struct sockaddr*>(&addr), &len) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "isPeerLocal ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TafNetSocketException(os.str());
    }
}

bool NetworkUtil::fdToRemoteAddress(int fd, struct sockaddr_in& addr)
{
    socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
    if(getpeername(fd, reinterpret_cast<struct sockaddr*>(&addr), &len) == SOCKET_ERROR)
    {
    	if(notConnected())
    	{
    	    return false;
    	}
    	else
    	{
    	    closeSocketNoThrow(fd);
            ostringstream os;
            os << "isPeerLocal ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TafNetSocketException(os.str());
    	}
    }
    return true;
}

string NetworkUtil::addrToString(const struct sockaddr_in& addr)
{
    ostringstream s;
    s << inetAddrToString(addr.sin_addr) << ':' << ntohs(addr.sin_port);
    return s.str();
}

