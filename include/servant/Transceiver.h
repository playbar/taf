#ifndef __TAF_TRANSCEIVER_H_
#define __TAF_TRANSCEIVER_H_

#include "servant/EndpointInfo.h"
#include "servant/Message.h"
#include "servant/Global.h"
#include "servant/NetworkUtil.h"
#include <list>

using namespace std;

namespace taf
{
/**
 * 网络传输基类，主要提供send/recv接口
 */
class Transceiver : public TC_HandleBase
{
public:
    /**
     * 构造函数
     * @param ep
     * @param fd
     */
    Transceiver(ObjectProxy *op, EndpointInfo ep, int fd);

    /**
     *
     *析构函数
     */
    virtual ~Transceiver();

    /**
     * 获取文件描述符
     * @return int
     */
    virtual int fd() const;

    /**
     * 关闭连接
     */
    virtual void doClose();

    /**
     * 是否有效
     * @return bool
     */
	virtual bool isValid() const;

	/**
	 * 获取连接的类型
	 * 
	 * @author kevintian (2010-8-25)
	 * 
	 * @return EndpointInfo::EType 
	 */
	EndpointInfo::EType getEndpointType() { return _ep.type(); }

public:
    /**
     * 处理请求，判断Send BufferCache是否有完整的包
     * @return int
     */
	virtual int doRequest();

    /**
     * 处理返回，判断Recv BufferCache是否有完整的包
     * @param done
     * @return int
     */
	virtual int doResponse(list<ResponsePacket>& done) = 0;

    /**
     * 发送到server的消息缓冲区
     *
     */
    void writeToSendBuffer(const string& msg);

    /**
     * 最后一个发送的请求
     * @param req
     */
    void setLastReq(ReqMessagePtr& req);

    /**
     * 关闭连接
     */
    void close();

public:
    /**
     * 网络发送接口
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int send(const void* buf, uint32_t len, uint32_t flag) = 0;

    /**
     * 网络接收接口
     * @param buf
     * @param len
     * @param flag
     *
     * @return int
     */
    virtual int recv(void* buf, uint32_t len, uint32_t flag) = 0;

protected:
    int _fd;

	EndpointInfo _ep;

    ObjectProxy *_objectProxy;

    string _sendBuffer;

    string _recvBuffer;

    ReqMessagePtr _lastReq;
};
//////////////////////////////////////////////////////////
/**
 * TCP 传输实现
 */
class TcpTransceiver : public Transceiver
{
public:
    /**
     * 构造函数
     * @param ep
     * @param fd
     */
    TcpTransceiver(ObjectProxy *op, EndpointInfo ep, int fd);

    /**
     * TCP 发送实现
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int send(const void* buf, uint32_t len, uint32_t flag);

    /**
     * TCP 接收实现
     * @param buf
     * @param len
     * @param flag
     *
     * @return int
     */
    virtual int recv(void* buf, uint32_t len, uint32_t flag);

    /**
     * 处理返回，判断Recv BufferCache是否有完整的包
     * @param done
     * @return int, =1,表示有数据就包
     */
	virtual int doResponse(list<ResponsePacket>& done);
};
//////////////////////////////////////////////////////////
/**
 * UDP 传输实现
 */
class UdpTransceiver : public Transceiver
{
public:
    /**
     * 构造函数
     * @param ep
     * @param fd
     */
    UdpTransceiver(ObjectProxy *op, EndpointInfo ep, int fd);

    /**
     * UDP 发送实现
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int send(const void* buf, uint32_t len, uint32_t flag);

    /**
     * UDP 接收实现
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int recv(void* buf, uint32_t len, uint32_t flag);

    /**
     * 处理返回，判断Recv BufferCache是否有完整的包
     * @param done
     * @return int
     */
	virtual int doResponse(list<ResponsePacket>& done);
};
//////////////////////////////////////////////////////////
}
#endif
