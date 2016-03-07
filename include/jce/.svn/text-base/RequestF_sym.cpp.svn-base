#include "./RequestF_sym.h"


namespace taf
{
    const taf::String & RequestPacket::className()
    {
        static taf::String sClassName((const signed char *)"taf.RequestPacket");
        return sClassName;
    }

    taf::String RequestPacket::structName() const
    {
        static taf::String sStructName((const signed char *)"taf.RequestPacket");
        return sStructName;
    }

    RequestPacket::RequestPacket()
    : iVersion(0),cPacketType(0),iMessageType(0),iRequestId(0),sServantName(taf::String((const signed char *)"")),sFuncName(taf::String((const signed char *)"")),iTimeout(0)
    {
    }

    void RequestPacket::reset()
    {
        iVersion = 0;
        cPacketType = 0;
        iMessageType = 0;
        iRequestId = 0;
        sServantName = taf::String((const signed char *)"");
        sFuncName = taf::String((const signed char *)"");
        sBuffer.clear();
        iTimeout = 0;
        context.clear();
        status.clear();
    }

    EXPORT_C void RequestPacket::writeTo(taf::JceOutputStream& _os) const
    {
        _os.write(iVersion, 1);
        _os.write(cPacketType, 2);
        _os.write(iMessageType, 3);
        _os.write(iRequestId, 4);
        _os.write(sServantName, 5);
        _os.write(sFuncName, 6);
        _os.write(sBuffer, 7);
        _os.write(iTimeout, 8);
        _os.write(context, 9);
        _os.write(status, 10);
    }

    EXPORT_C void RequestPacket::readFrom(taf::JceInputStream& _is)
    {
        _is.read(iVersion, 1, true);
        _is.read(cPacketType, 2, true);
        _is.read(iMessageType, 3, true);
        _is.read(iRequestId, 4, true);
        _is.read(sServantName, 5, true);
        _is.read(sFuncName, 6, true);
        _is.read(sBuffer, 7, true);
        _is.read(iTimeout, 8, true);
        _is.read(context, 9, true);
        _is.read(status, 10, true);
    }

    RequestPacket& RequestPacket::operator=(const RequestPacket& r)
    {
        this->iVersion = r.iVersion;
        this->cPacketType = r.cPacketType;
        this->iMessageType = r.iMessageType;
        this->iRequestId = r.iRequestId;
        this->sServantName = r.sServantName;
        this->sFuncName = r.sFuncName;
        this->sBuffer = r.sBuffer;
        this->iTimeout = r.iTimeout;
        this->context = r.context;
        this->status = r.status;
        return *this;
    }

    bool operator==(const RequestPacket& l, const RequestPacket& r)
    {
        return l.iVersion == r.iVersion && l.cPacketType == r.cPacketType && l.iMessageType == r.iMessageType && l.iRequestId == r.iRequestId && l.sServantName == r.sServantName && l.sFuncName == r.sFuncName && l.sBuffer == r.sBuffer && l.iTimeout == r.iTimeout && l.context == r.context && l.status == r.status;
    }

    bool operator!=(const RequestPacket&l, const RequestPacket&r)
    {
        return !(l == r);
    }


}



