#ifndef __TAF_ENDPOINT_INFO_H_
#define __TAF_ENDPOINT_INFO_H_

#include "servant/Global.h"
#include "servant/NetworkUtil.h"

using namespace std;

namespace taf
{
/**
 * 地址信息IP:Port
 */
class EndpointInfo
{
public:
	enum EType { TCP = 1, UDP = 2, };

    /**
     * 构造函数
     */
	EndpointInfo();

    /**
     * 构造函数
     * @param host
     * @param port
     * @param type
     */
	EndpointInfo(const string& host, uint16_t port, EndpointInfo::EType type, int32_t grid);

    /**
     * 地址的字符串描述
     *
     * @return string
     */
	string desc() const;

    /**
     * 获取主机名
     *
     * @return const string&
     */
	const string& host() const;

    /**
     * 获取端口号
     *
     * @return uint16_t
     */
	uint16_t port() const;

    /**
     * 获取路由状态
     * @return int32_t
     */
	int32_t grid() const;

    /**
     * 获取主机地址
     *
     * @return const struct sockaddr_in&
     */
	const struct sockaddr_in& addr() const;

    /**
     * 返回端口类型
     *
     * @return EndpointInfo::EType
     */
	EndpointInfo::EType type() const;

    /**
     * 赋值方法
     */
    EndpointInfo& operator = (const EndpointInfo& r);

    /**
     * 等于
     * @param r
     *
     * @return bool
     */
    bool operator == (const EndpointInfo& r) const;

    /**
     * 小于
     * @param r
     *
     * @return bool
     */
    bool operator < (const EndpointInfo& r) const;

private:
    /**
     * 地址IP
     */
    string _host;

    /**
     * 端口号
     */
    uint16_t _port;

    /**
     * 路由状态
     */
    int32_t _grid;

    /**
     * 类型
     */
	EndpointInfo::EType _type;

    /**
     * 地址
     */
	struct sockaddr_in _addr;
};
/////////////////////////////////////////////////////////////////////////////
}
#endif
