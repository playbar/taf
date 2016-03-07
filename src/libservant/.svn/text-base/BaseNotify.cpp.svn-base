#include "servant/BaseNotify.h"
#include "servant/NotifyObserver.h"

namespace taf
{

BaseNotify::BaseNotify() 
{
}

BaseNotify::~BaseNotify() 
{
}

void BaseNotify::addAdminCommandPrefix(const string& command, TAdminFunc func)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _procFunctors.insert(std::make_pair(command, func));

    NotifyObserver::getInstance()->registerPrefix(command, this);
}

void BaseNotify::addAdminCommandNormal(const string& command, TAdminFunc func)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _procFunctors.insert(std::make_pair(command, func));

    NotifyObserver::getInstance()->registerNotify(command, this);
}

bool BaseNotify::notify(const string& cmd, const string& params, JceCurrentPtr current, string& result)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    map<string, TAdminFunc>::iterator it;

    it =  _procFunctors.find(cmd);

    if (it != _procFunctors.end())
    {
        return (it->second)(cmd, params, result);
    }
    return false;
}
//////////////////////////////////////////////////////////////////
}
