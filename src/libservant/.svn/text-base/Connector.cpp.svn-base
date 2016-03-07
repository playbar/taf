#include "servant/Connector.h"

namespace taf
{

TransceiverPtr Connector::connect(ObjectProxy *op, const EndpointInfo &ep, int timeout)
{
	int fd = -1;

	TransceiverPtr trans = NULL;

	if (ep.type() == EndpointInfo::UDP)
	{
		if (ep.port() != 0)
		{
			fd = NetworkUtil::createSocket(true);

            NetworkUtil::setBlock(fd, false);

			trans = new UdpTransceiver(op, ep, fd);
		}
		else
		{
			fd = NetworkUtil::createSocket(true, true);

            NetworkUtil::setBlock(fd, false);

			unlink(ep.host().c_str());

			struct sockaddr_un addr;

			addr.sun_family = PF_LOCAL;

			strncpy(addr.sun_path, ep.host().c_str(), sizeof(addr.sun_path));
		
            //本地socket，好像有问题，不建议使用?
			NetworkUtil::doConnect(fd, (struct sockaddr_in&)(addr), timeout); //???

			NetworkUtil::doBind(fd, (struct sockaddr_in&)(addr)); //???
			
			trans = new UdpTransceiver(op, ep, fd);
		}
	}
	else
	{
		if (ep.port() != 0)
		{
			fd = NetworkUtil::createSocket(false);

			NetworkUtil::setBlock(fd, false);

			struct sockaddr_in addr = ep.addr();

			NetworkUtil::doConnect(fd, addr, timeout);

			trans = new TcpTransceiver(op, ep, fd);
		}
		else
		{
			fd = NetworkUtil::createSocket(false, true);

            NetworkUtil::setBlock(fd, false);

			struct sockaddr_un addr;

			addr.sun_family = PF_LOCAL;

			strncpy(addr.sun_path, ep.host().c_str(), sizeof(addr.sun_path));

			NetworkUtil::doConnect(fd, (struct sockaddr_in&)(addr), timeout);

			trans = new TcpTransceiver(op, ep, fd);
		}
	}
	return trans;
}
////////////////////////////////////////////////////////
}
