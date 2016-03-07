#include "servant/ServantHelper.h"

namespace taf
{

ServantPtr ServantHelperManager::create(const string &sAdapter)
{
    if(_adapter_servant.find(sAdapter) == _adapter_servant.end())
    {
        return NULL;
    }

    ServantPtr servant = NULL;

    //根据adapter查找servant名称
    string s = _adapter_servant[sAdapter];

    if(_servant_creator.find(s) != _servant_creator.end())
    {
        servant = _servant_creator[s]->create(s);
    }
    return servant;
}

void ServantHelperManager::setAdapterServant(const string &sAdapter, const string &sServant)
{
    _adapter_servant[sAdapter] = sServant;

    _servant_adapter[sServant] = sAdapter;
}

bool ServantHelperManager::setDyeing(const string & sDyeingKey, const string & sDyeingServant,
        const string & sDyeingInterface)
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);

    _sDyeingKey       = sDyeingKey;
    _sDyeingServant   = sDyeingServant;
    _sDyeingInterface = sDyeingInterface;

    _isDyeing         = !sDyeingKey.empty();

    return true;
}

bool ServantHelperManager::isDyeingReq(const string & sKey, const string & sServant, const string & sInterface)
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);

    return  ((_sDyeingKey == sKey) && (_sDyeingServant == sServant) &&
        (_sDyeingInterface == "" || _sDyeingInterface == sInterface) );
}

bool ServantHelperManager::setRouter(const string & sServant,ServantProxyRouterPtr router)
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);
    _mRouter[sServant] = router;
    return true;
}

int ServantHelperManager::getGridByKey(const string & sServant,const string & sKey)
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);

    map<string,ServantProxyRouterPtr>::const_iterator it = _mRouter.find(sServant);
    if(it == _mRouter.end() || !it->second )
    {
        return ThreadPrivateData::INVALID_GRID_CODE;
    }
    return it->second->getGridByKey(sKey);
}

}


