#ifndef _STRING_INPUT_STREAM_H
#define _STRING_INPUT_STREAM_H

#include <iostream>
#include <string>
#include <stdexcept>
#include "util/tc_ex.h"
#include "util/tc_autoptr.h"
#include "util/tc_clientsocket.h"

using namespace std;

namespace taf
{

/**
 * Hessian异常
 */
struct HessianException : public taf::TC_Exception
{
    HessianException(const string &buffer) : taf::TC_Exception(buffer){};
    ~HessianException() throw(){};
};

/**
 * 解码异常
 */
struct HessianProtocolException : public HessianException
{
    HessianProtocolException(const string &buffer) : HessianException(buffer){};
    ~HessianProtocolException() throw(){};
};

/**
 * 无有效service可用
 */
struct HessianNoActiveServiceException : public HessianException
{
    HessianNoActiveServiceException(const string &buffer) : HessianException(buffer){};
    ~HessianNoActiveServiceException() throw(){};
};

/**
 * 未知错误
 */
struct HessianUnknownException : public HessianException
{
    HessianUnknownException(const string &buffer) : HessianException(buffer){};
    ~HessianUnknownException() throw(){};
};

/**
 * 发送错误
 */
struct HessianSendException : public HessianException
{
    HessianSendException(const string &buffer) : HessianException(buffer){};
    ~HessianSendException() throw(){};
};

/**
 * Select异常
 */
struct HessianSelectException : public HessianException
{
    HessianSelectException(const string &buffer) : HessianException(buffer){};
    ~HessianSelectException() throw(){};
};

/**
 * 超时异常
 */
struct HessianTimeOutException : public HessianException
{
    HessianTimeOutException(const string &buffer) : HessianException(buffer){};
    ~HessianTimeOutException() throw(){};
};

/**
 * 接收数据异常
 */
struct HessianRecvException : public HessianException
{
    HessianRecvException(const string &buffer) : HessianException(buffer){};
    ~HessianRecvException() throw(){};
};

/**
 * 服务器关闭客户端句柄异常
 */
struct HessianLostCloseException : public HessianException
{
    HessianLostCloseException(const string &buffer) : HessianException(buffer){};
    ~HessianLostCloseException() throw(){};
};

/**
 * 连接异常
 */
struct HessianConnectException : public HessianException
{
    HessianConnectException(const string &buffer) : HessianException(buffer){};
    ~HessianConnectException() throw(){};
};

/**
 * Socket异常
 */
struct HessianSocketException : public HessianException
{
    HessianSocketException(const string &buffer) : HessianException(buffer){};
    ~HessianSocketException() throw(){};
};

/**
 * 抛出网络异常
 * @param ret
 */
inline void throwHessianException(int ret)
{
    switch(ret)
    {
    case taf::TC_ClientSocket::EM_SUCCESS:
        return;
    case taf::TC_ClientSocket::EM_SEND:
        throw HessianSendException("HessianSendException");
        break;
    case taf::TC_ClientSocket::EM_SELECT:
        throw HessianSelectException("HessianSelectException");
        break;
    case taf::TC_ClientSocket::EM_TIMEOUT:
        throw HessianTimeOutException("HessianTimeOutException");
        break;
    case taf::TC_ClientSocket::EM_RECV:
        throw HessianRecvException("HessianRecvException");
        break;
    case taf::TC_ClientSocket::EM_CLOSE:
        throw HessianLostCloseException("HessianLostCloseException");
        break;
    case taf::TC_ClientSocket::EM_CONNECT:
        throw HessianConnectException("HessianConnectException");
        break;
    case taf::TC_ClientSocket::EM_SOCKET:
        throw HessianSocketException("HessianSocketException");
        break;
    default:
        throw HessianUnknownException("HessianUnknownException");
        break;
    }
}

/**
 * 没有service异常
 */
struct HessianNoSuchHObjectException : public HessianException
{
    HessianNoSuchHObjectException(const string &buffer) : HessianException(buffer){};
    ~HessianNoSuchHObjectException() throw(){};
};

/**
 * 没有方法异常
 */
struct HessianNoSuchMethodException : public HessianException
{
    HessianNoSuchMethodException(const string &buffer) : HessianException(buffer){};
    ~HessianNoSuchMethodException() throw(){};
};

/**
 * 请求头异常
 */
struct HessianRequireHeaderException : public HessianException
{
    HessianRequireHeaderException(const string &buffer) : HessianException(buffer){};
    ~HessianRequireHeaderException() throw(){};
};

/**
 * Service抛出异常
 */
struct HessianServiceException : public HessianException
{
    HessianServiceException(const string &buffer) : HessianException(buffer){};
    ~HessianServiceException() throw(){};
};

/**
 * 读取string类,用于解析hessian协议
 */
class HessianStringStream : public taf::TC_HandleBase
{
public:
    /**
     * 构造函数
     * @param str
     */
	HessianStringStream(const string& str): _str(str), _pointer(0)
    {
    }

    /**
     * 是否到了尾部
     *
     * @return bool
     */
	bool eof()
    {
        return _pointer >= _str.length();
    }

    /**
     * 每次读取一个字节
     *
     * @return int
     */
	int read()
    {
    	if (eof())
    	{
    		throw HessianProtocolException("HessianProtocolException::read past end of stream");
    	}
    	char c;
    	c = _str[_pointer];
    	_pointer++;
    	return (int)c;
    }

    /**
     * 所有内容
     * 
     * @return string
     */
    string buffer() const { return _str; }

private:

    /**
     * 字符串流
     */
	string              _str;

    /**
     * 当前指针
     */
	string::size_type   _pointer;

};

typedef taf::TC_AutoPtr<HessianStringStream> HessianStringStreamPtr;

}

#endif

