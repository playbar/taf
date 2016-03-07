#include "servant/TMReactor.h"
#include "log/taf_logger.h"

using namespace std;

namespace taf
{

TMReactor::TMReactor()
: _terminate(false)
{
}

TMReactor::~TMReactor()
{
}

void TMReactor::terminate()
{
    TC_ThreadLock::Lock lock(*this);

    _terminate = true;

    notify();
}

void TMReactor::run()
{
    while (!_terminate)
    {
        try
        {
            TC_ThreadLock::Lock lock(*this);

            timedWait(100);

            time_t now = TC_TimeProvider::getInstance()->getNow();

            map<TMEventHandle*, TimerInfo>::iterator it;

            for (it = _handles.begin(); it != _handles.end(); ++it)
            {
                if (it->second.lastHandleTime + it->second.timeout <= (unsigned)now)
                {
                    it->second.lastHandleTime = now;

                    it->first->handleTimeout();
                }
            }
        }
        catch (exception& e)
        {
            LOG->error() << "[TAF][TMReactor:run exception:" << e.what() << "]" << endl;
        }
        catch (...)
        {
            LOG->error() << "[TAF][TMReactor:run exception.]" << endl;
        }
    }
}

void TMReactor::registerHandle(int timeout, TMEventHandle* handle)
{
    TC_ThreadLock::Lock lock(*this);

    TMReactor::TimerInfo info;

    info.timeout = timeout;

    info.lastHandleTime = TC_TimeProvider::getInstance()->getNow();

    _handles[handle] = info;
}

void TMReactor::unregisterHandle(int timeout, TMEventHandle* handle)
{
    TC_ThreadLock::Lock lock(*this);

    map<TMEventHandle*, TimerInfo>::iterator it;

    it = _handles.find(handle);

    if (it != _handles.end())
    {
        _handles.erase(it);
    }
}
//////////////////////////////////////////////////////////////////////////////////
}

