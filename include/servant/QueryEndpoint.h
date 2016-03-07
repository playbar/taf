#ifndef __TAF_QUERY_ENDPOINT_H_
#define __TAF_QUERY_ENDPOINT_H_

#include "servant/EndpointInfo.h"
#include "servant/EndpointF.h"
#include "servant/QueryF.h"
#include "servant/Global.h"

namespace taf
{
class QueryEndpoint : public QueryFPrxCallback, public TC_ThreadMutex
{
public:
    QueryEndpoint(const QueryFPrx& proxy);

    virtual ~QueryEndpoint();

public:
    virtual void callback_findObjectById4All(taf::Int32 ret, const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp);
    virtual void callback_findObjectById4All_exception(taf::Int32 ret);
    
public:
    virtual void callback_findObjectByIdInSameGroup(taf::Int32 ret, const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp);
    virtual void callback_findObjectByIdInSameGroup_exception(taf::Int32 ret);

public:
    set<EndpointInfo> findObjectById(const string& name);

public:
    void findObjectById4All(const string& name, bool sync);

	void findObjectByStation(const string& name, const string & sStation, set<EndpointInfo> & vecActive, set<EndpointInfo> & vecInactive);

    bool hasNewEndpoints(set<EndpointInfo>& activeEps, set<EndpointInfo>& inactiveEps, set<int32_t>& gridValues);

    inline bool hasRefreshed() {return _refreshed;}

    int setLocatorPrx(QueryFPrx prx);

protected:
    void fromEndpoints(const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp);

private:
    bool _refreshed;

    QueryFPrx _proxy;

    set<EndpointInfo> _activeEndpoints;

    set<EndpointInfo> _inactiveEndpoints;

    set<int32_t> _allGridValues;

    QueryFPrxCallbackPtr _self;
};
////////////////////////////////////////////////////////////////////////
}
#endif
