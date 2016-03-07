#ifndef __TAF_CONNECTOR_H_
#define __TAF_CONNECTOR_H_

#include "servant/EndpointInfo.h"
#include "servant/Transceiver.h"
#include "servant/NetworkUtil.h"
#include "servant/Global.h"

namespace taf
{

class Transceiver;
class ObjectProxy;

/**
 * 连接器，返回一个transceiver
 */
class Connector
{
public:

    /**
     * 建立连接
     * @param timeout
     *
     * @return TransceiverPtr
     */
    static TransceiverPtr connect(ObjectProxy *op, const EndpointInfo &ep, int timeout);
};
///////////////////////////////////////////////////
}
#endif
