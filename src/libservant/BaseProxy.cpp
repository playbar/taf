#include "servant/BaseProxy.h"
#include "servant/Message.h"
#include "servant/StatReport.h"

namespace taf
{

BaseProxy::BaseProxy(Communicator* comm)
: _comm(comm)
{
}

BaseProxy::~BaseProxy()
{
}

Communicator* BaseProxy::getCommunicator()
{
    return _comm;
}

void BaseProxy::reportToStat(ReqMessagePtr& req, const string& obj, int result)
{
    StatReport* stat = _comm->getStatReport();

    if (stat && stat->getStatPrx() && !_comm->isTerminating())
    {                
        int sptime = 10*1000;
                    
        //gettimeofday(&req->endtime, NULL);
        TC_TimeProvider::getInstance()->getNow(&req->endtime);

        if (req->begtime.tv_sec != 0
            && req->endtime.tv_sec != 0
            && req->endtime.tv_sec >= req->begtime.tv_sec)
        {
            sptime = (req->endtime.tv_sec - req->begtime.tv_sec)*1000;

            sptime += (req->endtime.tv_usec - req->begtime.tv_usec)/1000;
        }

        stat->report(obj, req->request.sFuncName, req->endpoint.host(), req->endpoint.port(), (StatReport::StatResult)result, sptime, 0, true);
    }
}

void BaseProxy::sampleToStat(ReqMessagePtr& req, const string& obj,const string &ip)
{
    StatReport* stat = _comm->getStatReport();

    if (stat && stat->getStatPrx() && !_comm->isTerminating())
    {      
        SET_MSG_TYPE(req->request.iMessageType, taf::JCEMESSAGETYPESAMPLE);     
                  
        stat->doSample(obj, req->request.sFuncName,ip,req->request.status);         
    }
}
///////////////////////////////////////////////////////////////////////////////
}
