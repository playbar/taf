#include "servant/ObjectProxyFactory.h"
#include "log/taf_logger.h"

namespace taf
{
ObjectProxyFactory::ObjectProxyFactory(Communicator* cm)
: _comm(cm) 
{
}

ObjectProxyFactory::~ObjectProxyFactory()
{
}

ObjectPrx::element_type* ObjectProxyFactory::getObjectProxy(const string& name)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    map<string, ObjectPrx>::iterator it = _objectProxys.find(name);

    if (it == _objectProxys.end())
    {
        int timeout = TC_Common::strto<int>(_comm->getProperty("async-invoke-timeout", "5000"));

        int refreshInterval = TC_Common::strto<int>(_comm->getProperty("refresh-endpoint-interval", "60000"));

        int cacheInterval = TC_Common::strto<int>(_comm->getProperty("cache-endpoint-interval", "1800000"));

        ObjectPrx op = new ObjectProxy(_comm, name);

        op->timeout(timeout);

        op->refreshEndpointInterval(refreshInterval);
        
        op->cacheEndpointInterval(cacheInterval); //缓存endpoint 供主控不可用时重启使用

        op->initialize();

        _objectProxys[name] = op;

        it = _objectProxys.find(name);
    }
    return it->second.get();
}

int ObjectProxyFactory::loadObjectLocator()
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    map<string, ObjectPrx>::iterator it;

    for (it = _objectProxys.begin(); it != _objectProxys.end(); ++it)
    {
        it->second->loadLocator();
    }
    return 0;
}
///////////////////////////////////////////////////////////////////
}
