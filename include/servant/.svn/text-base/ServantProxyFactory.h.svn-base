#ifndef __TAF_SERVANT_PROXY_FACTORY_H_
#define __TAF_SERVANT_PROXY_FACTORY_H_

#include "servant/Global.h"
#include "servant/Communicator.h"
#include "servant/ServantProxy.h"

namespace taf
{
/**
 * 创建ServantProxy对象，每个object在进程空间只有一个ServantProxy实例
 */
class ServantProxyFactory : public TC_HandleBase, public TC_ThreadRecMutex
{
public:
    /**
     * 构造函数
     * @param comm
     */
    ServantProxyFactory(Communicator* comm);

    /**
     * 析构
     */
    ~ServantProxyFactory();

    /**
     * 获取ServantProxy对象
     * @param name
     * @return ServantPrx
     */
    ServantPrx::element_type* getServantProxy(const string& name);

private:
    /**
     * 通信器
     */
    Communicator* _comm;

    /**
     * 已创建的对象
     */
    map<string, ServantPrx> _servantProxy;
};
//////////////////////////////////////////////////////
}
#endif
