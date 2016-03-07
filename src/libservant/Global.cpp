#include "servant/Global.h"
#include "servant/BaseF.h"

namespace taf
{

//////////////////////////////////////////////////////////////////////////
void TafException::throwException(int ret, const string& desc)
{
    switch (ret)
    {
    case taf::JCESERVERSUCCESS:
        break;
    case JCESERVERDECODEERR:
        throw TafServerDecodeException("server decode exception: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
        break;
    case JCESERVERENCODEERR:
        throw TafServerEncodeException("server encode exception: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
        break;
    case JCESERVERNOFUNCERR:
        throw TafServerNoFuncException("server function mismatch exception: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
        break;
    case JCESERVERNOSERVANTERR:
        throw TafServerNoServantException("server servant mismatch exception: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
        break;
    case JCESERVERRESETGRID:
        throw TafServerResetGridException("server reset grid exception: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
        break;
    case JCESERVERQUEUETIMEOUT:
        throw TafServerQueueTimeoutException("server queue timeout exception: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
        break;
    case JCEPROXYCONNECTERR:
        throw TafServerQueueTimeoutException("server connection lost: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
        break;
    default:
        throw TafServerUnknownException("server unknown exception: ret:" + TC_Common::tostr(ret) + " msg:"+ desc);
    }
}
////////////////////////////////////////////////////////////////////////////
}
