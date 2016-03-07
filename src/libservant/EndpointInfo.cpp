#include "servant/EndpointInfo.h"
#include "log/taf_logger.h"
namespace taf
{
EndpointInfo::EndpointInfo()
: _host("0.0.0.0")
, _port(0)
, _grid(0)
, _type(TCP)
{
}

EndpointInfo::EndpointInfo(const string& host, uint16_t port, EndpointInfo::EType type, int32_t grid)
: _host(host)
, _port(port)
, _grid(grid)
, _type(type)
{
	try
	{
		NetworkUtil::getAddress(_host, _port, _addr);
	}
	catch (...)
	{
		LOG->error() << "[ERROR:getAddress fail:" << _host << ":" << _port << "]" << endl;
	}
}

string EndpointInfo::desc() const
{ 
	ostringstream os; 

	if (_type == EndpointInfo::UDP) { os << "udp:"; }

	if (_type == EndpointInfo::TCP) { os << "tcp:"; }

	os << _grid << ":" << _host << ":" << _port;

	return os.str();
}

EndpointInfo& EndpointInfo::operator = (const EndpointInfo& r)
{
    if (this != &r)
    {
        _host = r._host;

        _addr = r._addr;

        _port = r._port;

        _type = r._type;

        _grid = r._grid;
    }
    return *this;
}

bool EndpointInfo::operator == (const EndpointInfo& r) const
{
    return (this->desc() == r.desc());
}

bool EndpointInfo::operator < (const EndpointInfo& r) const
{
    return (this->desc() < r.desc());
}

const string& EndpointInfo::host() const
{ 
    return _host;
}

int32_t EndpointInfo::grid() const
{
    return _grid;
}

uint16_t EndpointInfo::port() const
{ 
    return _port;
}

const struct sockaddr_in& EndpointInfo::addr() const
{ 
    return _addr;
}

EndpointInfo::EType EndpointInfo::type() const 
{ 
    return _type;
}
///////////////////////////////////////////////////////////
}
