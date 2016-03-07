#include "nodeF/taf_nodeF.h"
#include "log/taf_logger.h"
#include "servant/Communicator.h"

namespace taf
{

void TafNodeFHelper::setNodeInfo(const CommunicatorPtr &comm, const string &obj, const string &app, const string &server)
{
    _comm           = comm;
    if(!obj.empty())
    {
        _nodePrx        = _comm->stringToProxy<ServerFPrx>(obj);
    }

    _si.application = app;
    _si.serverName  = server;
    _si.pid         = getpid();
}

void TafNodeFHelper::keepAlive(const string &adapter)
{
        try
        {
            if(_nodePrx)
            {
                set<string> s;
                {
                    TC_LockT<TC_ThreadMutex> lock(*this);
    
                    _adapterSet.insert(adapter);
    
                    if(adapter != "AdminAdapter")
                    {
                        return;  
                    }
                    s.swap(_adapterSet);
                }
                ServerInfo si   = _si;
                set<string>::const_iterator it = s.begin();
                while(it != s.end())
                {                  
                    si.adapter      = *it; 
                    _nodePrx->keepAlive(si);
                    ++it;
                }

            }
        }
        catch(exception &ex)
        {
                LOG->error() << "TafNodeFHelper::keepAlive error:" << ex.what() << endl;
        }
        catch(...)
        {
                LOG->error() << "TafNodeFHelper::keepAlive unknown error" << endl;
        }
}

void TafNodeFHelper::reportVersion(const string &version)
{
        try
        {
        if(_nodePrx)
        {
            _nodePrx->async_reportVersion(NULL, _si.application, _si.serverName, version);
        }
        }
        catch(exception &ex)
        {
                LOG->error() << "TafNodeFHelper::reportVersion error:" << ex.what() << endl;
        }
        catch(...)
        {
                LOG->error() << "TafNodeFHelper::reportVersion unknown error" << endl;
        }
}

}

