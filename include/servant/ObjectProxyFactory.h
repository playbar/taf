#ifndef __TAF_OBJECT_PROXY_FACTORY_H_
#define __TAF_OBJECT_PROXY_FACTORY_H_

#include "servant/Communicator.h"
#include "servant/ObjectProxy.h"

namespace taf
{
/**
 * 获取ObjectProxy对象
 * 每个objectname在进程空间唯一对应一个objectproxy
 * object proxy 线程安全
 *
 */
class ObjectProxyFactory : public TC_HandleBase, public TC_ThreadRecMutex
{
public:
    /**
     * 构造函数
     * @param cm
     */
    ObjectProxyFactory(Communicator* cm);

    /**
     * 获取对象代理
     * @param name
     *
     * @return ObjectPrx
     */
    ObjectPrx::element_type* getObjectProxy(const string& name);

    /**
     *
     */
    ~ObjectProxyFactory();

    /**
     * 所有对象代理加载locator信息
     */
    int loadObjectLocator();

private:
    /**
     * 通信器
     */
    Communicator* _comm;

    /**
     * 保存已创建的objectproxy
     */
    map<string, ObjectPrx> _objectProxys;
};
/////////////////////////////////////////////////////
}
#endif
