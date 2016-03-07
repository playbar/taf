#include "util/tc_epoll_server.h"
#include "servant/AppProtocol.h"
#include "jce/Jce.h"
#include <iostream>

namespace taf
{

//TAFServer的协议解析器
int AppProtocol::parse(string &in, string &out)
{
    if(in.length() < sizeof(taf::Int32))
    {
        return TC_EpollServer::PACKET_LESS;
    }

    taf::Int32 iHeaderLen;

    memcpy(&iHeaderLen, in.c_str(), sizeof(taf::Int32));

    iHeaderLen = ntohl(iHeaderLen);
    
    if(iHeaderLen < taf::Int32(sizeof(taf::Int32))|| iHeaderLen > 100000000)
    {
        return TC_EpollServer::PACKET_ERR;
    }

    if((int)in.length() < iHeaderLen)
    {
        return TC_EpollServer::PACKET_LESS;
    }

    out = in.substr(sizeof(taf::Int32), iHeaderLen - sizeof(taf::Int32));

    in  = in.substr(iHeaderLen);

    return TC_EpollServer::PACKET_FULL;
}

int AppProtocol::parseAdmin(string &in, string &out)
{
    return parse(in, out);
}

/////////////////////////////////////////////////////////////////////////
size_t ProxyProtocol::wupResponse(const char* recvBuffer, size_t length, list<ResponsePacket>& done)
{
	size_t pos = 0;

    while (pos < length)
    {
        uint32_t len = length - pos;

        if(len < sizeof(taf::Int32))
        {
            break;
        }

        uint32_t iHeaderLen = ntohl(*(uint32_t*)(recvBuffer + pos));

        //做一下保护,长度大于10M
        if (iHeaderLen > 10000000)
        {
            throw JceDecodeException("packet length too long,len:" + TC_Common::tostr(iHeaderLen));
        }

        //包没有接收全
        if (len < iHeaderLen)
        {
            //看看包头是否正确
            static const uint32_t head = 20;

            if(len >= head)
            {
                JceInputStream<BufferReader> is;

                is.setBuffer(recvBuffer + pos + sizeof(taf::Int32), head);

                //wup回来是requestpackage
                RequestPacket rsp;

                is.read(rsp.iVersion, 1, true);

                if (rsp.iVersion != WUPVERSION && rsp.iVersion != WUPVERSION2)
                {
                    throw JceDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
                }

                is.read(rsp.cPacketType, 2, true);

                if (rsp.cPacketType != JCENORMAL)
                {
                    throw JceDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
                }

                is.read(rsp.iMessageType, 3, true);
                is.read(rsp.iRequestId, 4, true);
            }
			break;
        }
        else
        {
            JceInputStream<BufferReader> is;

            //buffer包括4个字节长度
            vector<char> buffer;
            buffer.resize(iHeaderLen);
            memcpy(&(buffer[0]), recvBuffer + pos, iHeaderLen);

            is.setBuffer(&(buffer[0]) + sizeof(taf::Int32), buffer.size() - sizeof(taf::Int32));

            pos += iHeaderLen;

            //WUP的响应包其实也是返回包
            RequestPacket req;
            req.readFrom(is);

            if (req.iVersion != WUPVERSION && req.iVersion != WUPVERSION2)
            {
                throw JceDecodeException("version not correct, version:" + TC_Common::tostr(req.iVersion));
            }

            if (req.cPacketType != JCENORMAL)
            {
                throw JceDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)req.cPacketType));
            }

            ResponsePacket rsp;
            rsp.cPacketType     = req.cPacketType;
            rsp.iMessageType    = req.iMessageType;
            rsp.iRequestId      = req.iRequestId;
            rsp.iVersion        = req.iVersion;
            //wup的响应包直接放入到sBuffer里面
            rsp.sBuffer         = buffer;

            done.push_back(rsp);
        }
    }

    return pos;
}

void ProxyProtocol::tafRequest(const RequestPacket& request, string& buff)
{
	JceOutputStream<BufferWriter> os;

	request.writeTo(os);

	taf::Int32 iHeaderLen = htonl(sizeof(taf::Int32) + os.getLength());

	buff.clear();

    buff.reserve(sizeof(taf::Int32) + os.getLength());

	buff.append((const char*)&iHeaderLen, sizeof(taf::Int32));

	buff.append(os.getBuffer(), os.getLength());
}

size_t ProxyProtocol::tafResponse(const char* recvBuffer, size_t length, list<ResponsePacket>& done)
{
	size_t pos = 0;

    while (pos < length)
    {
        uint32_t len = length - pos;

        if(len < sizeof(taf::Int32))
        {
            break;
        }

        uint32_t iHeaderLen = ntohl(*(uint32_t*)(recvBuffer + pos));

        //做一下保护,长度大于10M
        if (iHeaderLen > 10000000)
        {
            throw JceDecodeException("packet length too long,len:" + TC_Common::tostr(iHeaderLen));
        }

        //包没有接收全
        if (len < iHeaderLen)
        {
            //看看包头是否正确
            static const uint32_t head = 20;

            if(len >= head)
            {
                JceInputStream<BufferReader> is;

                is.setBuffer(recvBuffer + pos + sizeof(taf::Int32), head);

                ResponsePacket rsp;

                is.read(rsp.iVersion, 1, false);

                if (rsp.iVersion != JCEVERSION)
                {
                    throw JceDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
                }

                is.read(rsp.cPacketType, 2, false);

                if (rsp.cPacketType != JCENORMAL)
                {
                    throw JceDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
                }

                is.read(rsp.iRequestId, 3, false);

                is.read(rsp.iMessageType, 4, false);

                is.read(rsp.iRet, 5, false);

                if (rsp.iRet < JCESERVERUNKNOWNERR)
                {
                    throw JceDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
                }
            }
			break;
        }
        else
        {
            JceInputStream<BufferReader> is;

            is.setBuffer(recvBuffer + pos + sizeof(taf::Int32), iHeaderLen - sizeof(taf::Int32));

            pos += iHeaderLen;

            ResponsePacket rsp;

            rsp.readFrom(is);

            if (rsp.iVersion != JCEVERSION)
            {
                throw JceDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
            }

            if (rsp.cPacketType != JCENORMAL)
            {
                throw JceDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
            }

            if (rsp.iRet < JCESERVERUNKNOWNERR)
            {
                throw JceDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
            }

            done.push_back(rsp);
        }
    }

    return pos;
}
////////////////////////////////////////////////////////////////////////////////////
}

