#include "util/tc_common.h"
#include "util/tc_epoll_server.h"

#include <sys/un.h>
#include <iostream>

using namespace std;
using namespace taf;

extern TC_RollLogger g_logger;

/**
 * 处理类, 每个处理线程一个对象
 */
class EchoHandle : public TC_EpollServer::Handle
{
public:
    /**
     * 协议解析
     * @param in
     * @param out
     * 
     * @return int
     */
    static int parse(string &in, string &out)
    {
        if(in.at(in.length() - 1) == '\n')
        {
            out = in;
            in  = "";
            return TC_EpollServer::PACKET_FULL;
        }
        
        return TC_EpollServer::PACKET_LESS;
    }

    /**
     * 初始化
     */
    virtual void initialize()
    {
        cout << "initialize:" << pthread_self() << endl;
    }

    /**
     * 具体处理逻辑
     * @param stRecvData
     */
    virtual void handle(const TC_EpollServer::tagRecvData &stRecvData)
    {
     //   cout << stRecvData.buffer.length() << endl;
//     assert(stRecvData.buffer.length()==7193);
//        cout << "handle:" <<  stRecvData.buffer << endl;

        sendResponse(stRecvData.uid, stRecvData.buffer, stRecvData.ip, stRecvData.port);
    }

    /**
     * 过载保护
     * @param stRecvData
     */
    virtual void handleTimeout(const TC_EpollServer::tagRecvData &stRecvData)
    {
        cout << "EchoHandle::handleTimeout:" << pthread_self() << endl;
        close(stRecvData.uid);
    }
};

