#ifndef __REQUESTF_SYM_H_
#define __REQUESTF_SYM_H_

#include "Jce_sym.h"
#include "sys/types.h"
#include "netinet/in.h"


namespace taf
{
    struct RequestPacket : public taf::JceStructBase
    {
    public:
        static const taf::String & className();
        IMPORT_C taf::String structName() const;
        RequestPacket();
        void reset();
        IMPORT_C void writeTo(taf::JceOutputStream& _os) const;
        IMPORT_C void readFrom(taf::JceInputStream& _is);
    public:
        taf::Short iVersion;
        taf::Char cPacketType;
        taf::Int32 iMessageType;
        taf::Int32 iRequestId;
        taf::String sServantName;
        taf::String sFuncName;
        taf::JArray<taf::Char> sBuffer;
        taf::Int32 iTimeout;
        taf::JMapWrapper<taf::String, taf::String> context;
        taf::JMapWrapper<taf::String, taf::String> status;

        RequestPacket& operator=(const RequestPacket& r);
    };
    bool operator==(const RequestPacket& l, const RequestPacket& r);
    bool operator!=(const RequestPacket&l, const RequestPacket&r);

}

#endif
