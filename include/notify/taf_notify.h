#ifndef __TAF_NOTIFY_H_
#define __TAF_NOTIFY_H_

#include "notify/Notify.h"
#include "servant/Global.h"
#include "util/tc_singleton.h"

using namespace std;

namespace taf
{

/**
 * 上报信息给Notify服务
 * 异步上报给notify服务
 */
class TafRemoteNotify : public TC_Singleton<TafRemoteNotify>
{
public:
    /**
     * 初始化
     * @param comm, 通信器
     * @param obj, 对象名称
     * @param notifyPrx
     * @param app
     * @param serverName
     *
     * @return int
     */
	int setNotifyInfo(const CommunicatorPtr &comm, const string &obj, const string & app, const string &serverName);

    /**
     * 通知, 一定是异步上报的
     * @param message
     */
    void notify(NOTIFYLEVEL level, const string &sMesage);

    /**
     * 上报
     * @param sResult
     * @param bSync
     */
    void report(const string &sResult, bool bSync = false);

protected:
    /**
     * 通信器
     */
    CommunicatorPtr _comm;

    /**
     * 通知代理
     */
	NotifyPrx       _notifyPrx;

    /**
     * 应用
     */
    string          _sApp;

    /**
     * 服务名称
     */
	string          _sServerName;
};

}

#endif
