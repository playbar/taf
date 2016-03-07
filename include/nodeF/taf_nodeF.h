#ifndef __TAF_NODEF_H__
#define __TAF_NODEF_H__

#include "nodeF/NodeF.h"
#include "servant/Global.h"
#include "util/tc_singleton.h"

namespace taf
{

/**
 * 给node发送心跳
 * 调用keepAlive异步发送心跳给node
 */
class TafNodeFHelper : public TC_Singleton<TafNodeFHelper>,public TC_ThreadMutex 
{
public:

    /**
     * 设置node信息
     * @param comm, 通信器
     * @param obj,
     * @param app
     * @param server
     */
    void setNodeInfo(const CommunicatorPtr &comm, const string &obj, const string &app, const string &server);

    /**
     * keepAlive
     */
    void keepAlive(const string &adapter = "");

    /**
     * 上报TAF的编译版本
     * @param version
     */
    void reportVersion(const string &version);

protected:
    /**
     * 通信器
     */
    CommunicatorPtr _comm;

    /**
     * Node
     */
    ServerFPrx      _nodePrx;

    /**
     * 信息
     */
    ServerInfo      _si;

    set<string>     _adapterSet;

};

}

#endif

