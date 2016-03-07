#ifndef __TAF_JCE_CURRENT_H_
#define __TAF_JCE_CURRENT_H_

#include "util/tc_epoll_server.h"
#include "jce/RequestF.h"
#include "servant/BaseF.h"

namespace taf
{
    
class ServantHandle;

/**
 * 当前请求的上下文
 */
class JceCurrent : public TC_HandleBase
{
public:
    typedef std::map<string, string>    TAF_STATUS;

    typedef std::vector<char>           TAF_BUFFER;

    /**
     * 构造函数
     * @param pServantHandle
     */
    JceCurrent(ServantHandle *pServantHandle);

    /**
     * 析构
     */
    ~JceCurrent();

    /**
     * 获取IP
     * @return string
     */
    string getIp() const;

    /**
     * 获取端口
     * @return int
     */
    int getPort() const;

    /**
     * 是否函数返回时发送响应包给客户端
     * @return bool
     */
    bool isResponse() const;

    /**
     * 设置是否自动回响应包
     */
    void setResponse(bool value) { _bResponse = value; }

    /**
     * 关闭当前连接
     */
    void close();

    /**
     * 获取所属的ServantHandle
     */
    ServantHandle* getServantHandle();

    /**
     * 获取来源的Adapter
     * @return TC_EpollServer::BindAdapter* 
     */
    TC_EpollServer::BindAdapter* getBindAdapter();

    /**
     * 获取请求buffer
     * @return string
     */
    const vector<char> &getRequestBuffer() const;

    /**
     * 获取服务Servant名称
     * @return string
     */
    string getServantName() const;

    /**
     * 请求的协议的版本号(仅TAF协议有效)
     * 
     * @return short
     */
    short getRequestVersion() const;

    /**
     * 扩展map(仅TAF协议有效)
     * @return map<string,string>&
     */
    map<string, string>& getContext();

    /**
     * 保存状态信息，比如灰度、染色等(仅TAF协议有效)
     * @return map<string,string>&
     */
    const map<string, string>& getRequestStatus() const;

    /**
     * 函数名称(仅TAF协议有效)
     * @return string
     */
    string getFuncName() const;

    /**
     * 请求ID(仅TAF协议有效)
     * @return int
     */
    uint32_t getRequestId() const;


    /**
     * 获取包类型(仅TAF协议有效)
     * @return char
     */
    char getPacketType() const;

    /**
     * 获取消息类型(仅TAF协议有效)
     * @return taf::Int32
     */
    taf::Int32 getMessageType() const;

	/**
	 * 获取接收到请求的时间
	 */
	struct timeval getRecvTime() const;


    /**
     * taf协议的发送响应数据(仅TAF协议有效)
     * @param iRet
     * @param status
     * @param buffer
     */
    void sendResponse(int iRet, const vector<char>& buffer = TAF_BUFFER(), 
                      const map<string, string>& status = TAF_STATUS(),
                      const string & sResultDesc = "");

    /**
     * 普通协议的发送响应数据(非TAF协议有效)
     * @param buff 
     * @param len 
     */
    void sendResponse(const char* buff, uint32_t len);

protected:

    friend class ServantHandle;

    friend class Application;

    /**
     * 初始化
     * @param stRecvData
     */
    void initialize(const TC_EpollServer::tagRecvData &stRecvData);

    /**
     * 初始化
     * @param sRecvBuffer
     */
    void initialize(const string &sRecvBuffer);

    /**
     * 服务端上报状态，针对单向调用及WUP调用(仅对TAF协议有效)
     */
    void reportToStat(const string & sObj);

    /**
    * 保存采样信息(仅对TAF协议有效)
    */
    void saveSampleKey();

protected:
    /**
     * 操作类指针
     */
    ServantHandle* _pServantHandle;

    /**
     * 消息_pBindAdapter
     */
    TC_EpollServer::BindAdapter* _pBindAdapter;

    /**
     * 唯一标识
     */
    uint32_t                _uid;

    /**
     * ip地址
     */
    string                  _ip;

    /**
     * 端口
     */
    int                     _port;

    /**
     * 客户端请求包
     */
    RequestPacket           _request;

    /**
     * 响应
     */
    bool                    _bResponse;

    /**
     * 收到请求时间
     */
    timeval                 _begintime;

    /**
     * 接口处理的返回值
     */
    int                     _iRet;
};
//////////////////////////////////////////////////////////////
}
#endif
