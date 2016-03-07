#include "servant/ServantProxyFactory.h"
#include "log/taf_logger.h"

namespace taf
{

ServantProxyFactory::ServantProxyFactory(Communicator* cm)
: _comm(cm)
{
}

ServantProxyFactory::~ServantProxyFactory()
{
}

ServantPrx::element_type* ServantProxyFactory::getServantProxy(const string& name)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

	map<string, ServantPrx>::iterator it = _servantProxy.find(name);

	if (it == _servantProxy.end())
	{
		ObjectPrx::element_type* op = _comm->objectProxyFactory()->getObjectProxy(name);

		ServantPrx sp = new ServantProxy(_comm, op);

        int timeout = TC_Common::strto<int>(_comm->getProperty("sync-invoke-timeout", "3000"));

        sp->taf_timeout(timeout);

		_servantProxy[name] = sp;

		it = _servantProxy.find(name);
	}
	return it->second.get();
}
///////////////////////////////////////////////////////////////////////////////
}
