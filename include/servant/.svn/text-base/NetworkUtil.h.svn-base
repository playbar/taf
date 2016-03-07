#ifndef __TAF_NETWORK_UTIL_H_
#define __TAF_NETWORK_UTIL_H_

#ifdef __hpux
#   define _XOPEN_SOURCE_EXTENDED
#endif

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <vector>

#include "util/tc_thread_mutex.h"

#ifndef SHUT_RD
#   define SHUT_RD 0
#endif

#ifndef SHUT_WR
#   define SHUT_WR 1
#endif

#ifndef SHUT_RDWR
#   define SHUT_RDWR 2
#endif

#ifndef NETDB_INTERNAL
#   define NETDB_INTERNAL -1
#endif

#ifndef NETDB_SUCCESS
#   define NETDB_SUCCESS 0
#endif

namespace taf
{
struct NetworkUtil
{
    static TC_ThreadMutex __mutex;
    static const int INVALID_SOCKET = -1;
    static const int SOCKET_ERROR = -1;

    static bool interrupted();
    static bool acceptInterrupted();
    static bool noBuffers();
    static bool wouldBlock();
    static bool connectFailed();
    static bool connectionRefused();
    static bool connectInProgress();
    static bool connectionLost();
    static bool notConnected();
    static bool recvTruncated();

    static int createSocket(bool, bool isLocal = false);
    static void closeSocket(int);
    static void closeSocketNoThrow(int);
    static void shutdownSocketWrite(int);
    static void shutdownSocketReadWrite(int);
    static void setBlock(int, bool);
    static void setTcpNoDelay(int);
    static void setKeepAlive(int);
    static void setSendBufferSize(int, int);
    static int getSendBufferSize(int);
    static void setRecvBufferSize(int, int);
    static int getRecvBufferSize(int);
    static void doBind(int, struct sockaddr_in&);
    static void doListen(int, int);
    static void doConnect(int, struct sockaddr_in&, int);

    static void getAddress(const std::string&, int, struct sockaddr_in&);
    static std::string errorToString(int);
    static std::string lastErrorToString();
    static std::string fdToString(int);
    static void fdToLocalAddress(int, struct sockaddr_in&);
    static bool fdToRemoteAddress(int, struct sockaddr_in&);
    static std::string addrToString(const struct sockaddr_in&);
    static int getSocketErrno();
    static std::string inetAddrToString(const struct in_addr&);
};
}
#endif
