#include "servant/AdminServant.h"
#include "servant/Application.h"
#include "servant/NotifyObserver.h"

namespace taf
{

AdminServant::AdminServant()
{
}

AdminServant::~AdminServant()
{
}

void AdminServant::initialize()
{
}

void AdminServant::destroy()
{
}

void AdminServant::shutdown(JceCurrentPtr current)
{
    Application::terminate();
}

string AdminServant::notify(const string &command, JceCurrentPtr current)
{
    TafRemoteNotify::getInstance()->report("AdminServant::notify:" + command);

    return NotifyObserver::getInstance()->notify(command, current);
}
///////////////////////////////////////////////////////////////////////
}
