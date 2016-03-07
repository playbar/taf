#include "servant/QueryEndpoint.h"
#include "log/taf_logger.h"

namespace taf
{

QueryEndpoint::QueryEndpoint(const QueryFPrx& proxy)
: _refreshed(false)
, _proxy(proxy)
, _self(this)
{
}

QueryEndpoint::~QueryEndpoint()
{
}

void QueryEndpoint::callback_findObjectById4All(taf::Int32 ret, const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp)
{
    LOG->info() << "[TAF][callback_findObjectById4All ret:" << ret << ",active:" << activeEp.size() << ",inactive:" << inactiveEp.size() << "]" << endl;

    if (ret == 0)
    {
        fromEndpoints(activeEp, inactiveEp);
    }
}

void QueryEndpoint::callback_findObjectById4All_exception(taf::Int32 ret)
{
    LOG->error() << "[TAF][callback_findObjectById4All_exception ret:" << ret << "]" << endl;
}

void QueryEndpoint::callback_findObjectByIdInSameGroup(taf::Int32 ret, const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp)
{
    LOG->info() << "[TAF][callback_findObjectByIdInSameGroup ret:" << ret << ",active:" << activeEp.size() << ",inactive:" << inactiveEp.size() << "]" << endl;

    if (ret == 0)
    {
        fromEndpoints(activeEp, inactiveEp);
    }
}

void QueryEndpoint::callback_findObjectByIdInSameGroup_exception(taf::Int32 ret)
{
    LOG->error() << "[TAF][callback_findObjectByIdInSameGroup_exception ret:" << ret << "]" << endl;
}

void QueryEndpoint::fromEndpoints(const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    _activeEndpoints.clear();

    _inactiveEndpoints.clear();

    _allGridValues.clear();

    for (uint32_t i = 0; i < activeEp.size(); ++i)
    {
        EndpointInfo::EType type = (activeEp[i].istcp ? EndpointInfo::TCP : EndpointInfo::UDP);

        EndpointInfo ep(activeEp[i].host, activeEp[i].port, type, activeEp[i].grid);

        _activeEndpoints.insert(ep);

        _allGridValues.insert(activeEp[i].grid);
    }

    for (uint32_t i = 0; i < inactiveEp.size(); ++i)
    {
        EndpointInfo::EType type = (inactiveEp[i].istcp ? EndpointInfo::TCP : EndpointInfo::UDP);

        EndpointInfo ep(inactiveEp[i].host, inactiveEp[i].port, type, inactiveEp[i].grid);

        _inactiveEndpoints.insert(ep);

        _allGridValues.insert(inactiveEp[i].grid);
    }
    _refreshed = true;
}

set<EndpointInfo> QueryEndpoint::findObjectById(const string& name)
{
    set<EndpointInfo> activeEndpoints;
	
    try
    {
        LOG->info() << "[TAF][findObjectById:" << name << "]" << endl;

        vector<taf::EndpointF> activeEp;

        activeEp = _proxy->findObjectById(name);

        for (uint32_t i = 0; i < activeEp.size(); ++i)
        {
            EndpointInfo::EType type = (activeEp[i].istcp ? EndpointInfo::TCP : EndpointInfo::UDP);

            EndpointInfo ep(activeEp[i].host, activeEp[i].port, type, activeEp[i].grid);

            activeEndpoints.insert(ep);
        }
    }
    catch (exception& e)
    {
        LOG->error() << "[TAF][findObjectById registry fail:" << name << "]:" << e.what() << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][findObjectById registry fail:" << name << "]" << endl;
    }
    return activeEndpoints;
}


void QueryEndpoint::findObjectById4All(const string& name, bool sync = true)
{
    try
    {
        if (sync)
        {
            LOG->info() << "[TAF][findObjectById4All:" << name << "]" << endl;

            vector<taf::EndpointF> activeEp;

            vector<taf::EndpointF> inactiveEp;

            _proxy->findObjectByIdInSameGroup(name, activeEp, inactiveEp);

            fromEndpoints(activeEp, inactiveEp);
        }
        else
        {
            LOG->info() << "[TAF][async_findObjectById4All:" << name << "]" << endl;

            _proxy->async_findObjectByIdInSameGroup(_self, name);
        }
    }
    catch (exception& e)
    {
        LOG->error() << "[TAF][findObjectById4All registry fail:" << name << "]:" << e.what() << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][findObjectById4All registry fail:" << name << "]" << endl;
    }
}

void QueryEndpoint::findObjectByStation(const string& name, const string & sStation, set<EndpointInfo> & setActive, set<EndpointInfo> & setInactive)
{
    setActive.clear();
    setInactive.clear();

    try
    {
        vector<taf::EndpointF> activeEp;
        vector<taf::EndpointF> inactiveEp;
        _proxy->findObjectByIdInSameStation(name, sStation, activeEp, inactiveEp);

        for (vector<taf::EndpointF>::size_type i = 0; i < activeEp.size(); ++i)
        {
            EndpointInfo::EType type = (activeEp[i].istcp ? EndpointInfo::TCP : EndpointInfo::UDP);

            EndpointInfo ep(activeEp[i].host, activeEp[i].port,  type, activeEp[i].grid);

            setActive.insert(ep);
        }

        for (uint32_t i = 0; i < inactiveEp.size(); ++i)
        {
            EndpointInfo::EType type = (inactiveEp[i].istcp ? EndpointInfo::TCP : EndpointInfo::UDP);

            EndpointInfo ep(inactiveEp[i].host, inactiveEp[i].port, type, inactiveEp[i].grid);

            setInactive.insert(ep);
        }
    }
    catch (exception& e)
    {
        LOG->error() << "[TAF][findObjectByStation registry fail:" << name << "]:" << e.what() << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][findObjectByStation registry fail:" << name << "]" << endl;
    }
}

bool QueryEndpoint::hasNewEndpoints(set<EndpointInfo>& activeEps, set<EndpointInfo>& inactiveEps, set<int32_t>& gridValues)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    if (_refreshed)
    {
        activeEps = _activeEndpoints;

        inactiveEps = _inactiveEndpoints;

        gridValues = _allGridValues;

        _refreshed = false;

        return true;
    }
    return false;
}

int QueryEndpoint::setLocatorPrx(QueryFPrx prx)
{
    _proxy = prx;

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
}
