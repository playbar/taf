#include "notify/taf_notify.h"
#include "servant/Communicator.h"
#include "log/taf_logger.h"

namespace taf
{

int TafRemoteNotify::setNotifyInfo(const CommunicatorPtr &comm, const string &obj, const string & app, const string &serverName)
{
    _comm           = comm;
    if(!obj.empty())
    {
        _notifyPrx  = _comm->stringToProxy<NotifyPrx>(obj);
        _notifyPrx->taf_timeout(500);
    }

    _sApp           = app;
    _sServerName    = serverName;
    return 0;
}

void TafRemoteNotify::report(const string &sResult, bool bSync)
{
    try
    {
        if(_notifyPrx)
        {
            if(!bSync)
            {
                _notifyPrx->async_reportServer(NULL, _sApp + "." + _sServerName, TC_Common::tostr(pthread_self()), sResult);
            }
            else
            {
                _notifyPrx->reportServer(_sApp + "." + _sServerName, TC_Common::tostr(pthread_self()), sResult);
            }
        }
    }
	catch(exception &ex)
	{
		LOG->error() << "TafRemoteNotify::report error:" << ex.what() << endl;
	}
	catch(...)
	{
		LOG->error() << "TafRemoteNotify::report unknown error" << endl;
	}
}

void TafRemoteNotify::notify(NOTIFYLEVEL level, const string &sMessage)
{
    try
    {
        if(_notifyPrx)
        {
            _notifyPrx->async_notifyServer(NULL, _sApp + "." + _sServerName, level, sMessage);
        }
    }
	catch(exception &ex)
	{
		LOG->error() << "TafRemoteNotify::notify error:" << ex.what() << endl;
	}
	catch(...)
	{
		LOG->error() << "TafRemoteNotify::notify unknown error" << endl;
	}
}

}


