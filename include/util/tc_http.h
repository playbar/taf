#ifndef __TC_HTTP_H_
#define __TC_HTTP_H_

#include "util/tc_ex.h"
#include "util/tc_common.h"
#include "util/tc_autoptr.h"
#include "util/tc_thread.h"
#include "util/tc_socket.h"
#include "util/tc_epoller.h"
#include "util/tc_timeout_queue.h"
#include <map>
#include <sstream>
#include <cassert>
#include <vector>

namespace taf
{
/////////////////////////////////////////////////
// 说明: http类
// 1 支持http协议的解析,支持GET/POST模式
// 2 内嵌支持对chunk编码的解析
// 3 支持同步http请求和响应(只支持短连接,不支持keep-alive)
// 4 支持http响应包体的增量解析
// 5 增加Cookie管理类
// Author : j@syswin.com              
// Modify: @syswin.com              
/////////////////////////////////////////////////

/**
* http协议解析异常类
*/
struct TC_Http_Exception : public TC_Exception
{
    TC_Http_Exception(const string &sBuffer) : TC_Exception(sBuffer){};
    ~TC_Http_Exception() throw(){};
};

/**
 * http应答协议解析异常类
 */
struct TC_HttpResponse_Exception : public TC_Http_Exception
{
    TC_HttpResponse_Exception(const string &sBuffer) : TC_Http_Exception(sBuffer){};
    ~TC_HttpResponse_Exception() throw(){};
};

/**
 * http请求协议解析异常类
 */
struct TC_HttpRequest_Exception : public TC_Http_Exception
{
    TC_HttpRequest_Exception(const string &sBuffer) : TC_Http_Exception(sBuffer){};
    ~TC_HttpRequest_Exception() throw(){};
};

/**
 * 简单的URL解析类
 * ftp://user:password@www.qq.com:8080/abc/dev/query?a=b&c=3#ref
 * scheme: ftp
 * user:user
 * pass:password
 * domain:www.qq.com
 * port:8080
 * path:/abc/dev/query
 * query:a=b&c=3
 * ref:ref
 * request:/abc/dev/query?a=b&c=3#ref
 * relativePath:/abc/dev/
 * rootPath:ftp://user:password@www.qq.com:8080/
 */

class TC_URL
{
public:
    enum URL_TYPE
    {
        HTTP,
        HTTPS,
        FTP
    };

    /**
     * 构造
     */
    TC_URL() : _iURLType(HTTP)
    {
    }

    /**
     * 解析URL, url必须是绝对路径
     * @throws TC_URL_Exception
     * @param sURL
     */
    bool parseURL(const string &sURL);

    /**
     * 清空
     */
    void clear();

    /**
     * 获取scheme
     * 
     * @return const string&
     */
    string getScheme() const;

    /**
     * 获取用户名
     * 
     * @return const string&
     */
    string getUserName() const;

    /**
     * 获取密码
     * 
     * @return const string&
     */
    string getPassword() const;

    /**
     * 获取域名
     * 
     * @return const string&
     */
    string getDomain() const;

    /**
     * 获取端口
     * 
     * @return const string&
     */
    string getPort() const;

    /**
     * 是否是缺省的端口
     * 
     * @return bool
     */
    bool isDefaultPort() const;

    /**
     * 获取路径
     * 
     * @return const string&
     */
    string getPath() const;

    /**
     * 获取查询部分
     * 
     * @return string
     */
    string getQuery() const;

    /**
     * 不包括Host
     * 例如http://www.qq.com/abc?a=b#ref则为:/abc?a=b#ref
     *
     * @return string
     */
    string getRequest() const;

    /**
     * 获取Ref
     * 
     * @return const string&
     */
    string getRef() const;

    /**
     * 是否有效
     * 
     * @return bool
     */
    bool isValid() const;

    /**
     * 获取解析后URL
     * 
     * @return const string&
     */
    string getURL() const;

    /**
     * URL类型
     * 
     * @return int
     */
    int getType() const { return _iURLType; }

    /**
     * 获取相对路径
     * 
     * @return string
     */
    string getRelativePath() const;

    /**
     * 获取根路径
     * 
     * @return string
     */
    string getRootPath() const;

    /**
     * 以当前URL调整路径 
     * 1 http://www.qq.com/a/b?a=b, /test/abc => http://www.qq.com/test/abc
     * 2 http://www.qq.com/a/b?a=b, test/abc   => http://www.qq.com/a/b/test/abc
     * 3 http://www.qq.com/a/b?a=b, ../test/abc   => http://www.qq.com/a/test/abc
     * 
     * @param sRelativeURL: 相对当前URL的地址
     */
    TC_URL buildWithRelativePath(const string &sRelativeURL) const;

    /**
     * 规整化
     */
    void specialize();

protected:
    /**
     * 换成URL
     * 
     * @return string
     */
    string toURL() const;

    /**
     * 
     * @param frag
     * 
     * @return string
     */
    string getFragment(const string& frag) const;

    /**
     * 类型到字符串
     * 
     * @return string
     */
    string type2String() const;

    /**
     * 获取协议的缺省端口
     * 
     * @return string
     */
    string getDefaultPort() const;

    /**
     * 简化URL
     * @param sPath
     * 
     * @return string
     */
    string simplePath(const string &sPath) const;

protected:
    /**
     * 解析后的URL
     */
    string              _sURL;

    /**
     * URL类型
     */
    int                 _iURLType;

    /**
     * 每一段
     */
    map<string, string> _fragment;
};

/**
 * http协议解析类, 请求和响应都在该类中解析
 */
class TC_Http
{
public:
    /**
     * 构造函数
     */
    TC_Http()
    {
        TC_Http::reset();
        setConnection("close");     //默认就用短连接
    }

    //不区分头部大小写
    struct CmpCase
    {
        bool operator()(const string &s1, const string &s2) const
        {
            return TC_Common::upper(s1) < TC_Common::upper(s2);
        }
    };

    typedef multimap<string, string, CmpCase> http_header_type;

    /**
     * 删除头部
     * @param sHeader
     */
    void eraseHeader(const string &sHeader) { _headers.erase(sHeader); }

    /**
     * 设置 Cache-Control
     * @param sCacheControl
     */
    void setCacheControl(const string &sCacheControl){setHeader("Cache-Control", sCacheControl);}

    /**
     * 设置 Connection
     * @param sConnection
     */
    void setConnection(const string &sConnection){setHeader("Connection", sConnection);}

    /**
     * 设置 Content-Type
     * @param sContentType
     */
    void setContentType(const string &sContentType){setHeader("Content-Type", sContentType);}

    /**
     * 设置内容长度
     * @param iContentLength
     */
    void setContentLength(size_t iContentLength)
    {
        setHeader("Content-Length", TC_Common::tostr(iContentLength));
    }

    /**
     * 设置 Referer
     * @param sReferer
     */
    void setReferer(const string &sReferer){setHeader("Referer", sReferer);}

    /**
     * 设置 Host, 例如 www.qq.com:80
     * @param sHost
     */
    void setHost(const string &sHost){setHeader("Host", sHost);}

    /**
     * 设置 Accept-Encoding
     * @param sAcceptEncoding, gzip, compress, deflate, identity
     */
    void setAcceptEncoding(const string &sAcceptEncoding){setHeader("Accept-Encoding", sAcceptEncoding);}

    /**
     * 设置 Accept-Language
     * @param sAcceptLanguage
     */
    void setAcceptLanguage(const string &sAcceptLanguage){setHeader("Accept-Language", sAcceptLanguage);}

    /**
     * 设置 Accept
     * @param sAccept
     */
    void setAccept(const string &sAccept){setHeader("Accept", sAccept);}

    /**
     * 设置 Transfer-Encoding
     * @param sTransferEncoding
     */
    void setTransferEncoding(const string& sTransferEncoding) {setHeader("Transfer-Encoding", sTransferEncoding); }

    /**
     * 设置header.常用的值请使用已经有的get/set方法设置 
     * (除Set-Cookie&Cookie外,头部字段不能重复)
     * @param sHeadName 
     * @param sHeadValue 
     */
    void setHeader(const string &sHeadName, const string &sHeadValue) 
    {
        //Set-Cookie和Cookie可以有多个头
        if(TC_Common::upper(sHeadName) != "SET-COOKIE" && TC_Common::upper(sHeadName) != "COOKIE")
        {
            _headers.erase(sHeadName);
        }
        _headers.insert(multimap<string, string>::value_type(sHeadName, sHeadValue)); 
    }

    /**
     * 设置header.常用的值请使用已经有的get/set方法设
     * (头部字段可以重复)
     * @param sHeadName
     * @param sHeadValue 
     */
    void setHeaderMulti(const string &sHeadName, const string &sHeadValue) 
    {
        _headers.insert(multimap<string, string>::value_type(sHeadName, sHeadValue)); 
    }

    /**
     * 获取头(重复的也提取)
     * @param sHeadName
     * 
     * @return vector<string>
     */
    vector<string> getHeaderMulti(const string &sHeadName) const;

    /**
     * 获取http内容长度
     *
     * @return size_t
     */
    size_t getContentLength() const;

    /**
     * 获取http请求Host
     *
     * @return string
     */
    string getHost() const;

    /**
     * 获取http头部长度
     *
     * @return size_t
     */
    size_t getHeadLength() const { return _headLength; }

    /**
     * 获取http内容
     *
     * @return string
     */
    string getContent() const { return _content; }

    /**
     * 设置http body(默认不修改content-length)
     * @param content
     * @param bUpdateContentLength, 是否更新ContentLength
     */
    void setContent(const string &content, bool bUpdateContentLength = false) 
    {
        _content = content; 

        if(bUpdateContentLength)
        {
            eraseHeader("Content-Length");
            if(_content.length() > 0)
                setContentLength(_content.length());
        }
    }

    /**
     * 获取内容类型
     *
     * @return string
     */
    string getContentType() const;

    /**
     * 获取http头部参数
     * (Set-Cookie和Cookie不要用这个获取, 单独有函数获取)
     * @param sHeader
     *
     * @return string
     */
    string getHeader(const string& sHeader) const;

    /**
     * 获取http头部map
     *
     * @return http_header_type&
     */
     const http_header_type& getHeaders() const{return _headers;}

     /**
      * 重置
      */
     void reset();

    /**
     * 读取一行
     * @param ppChar
     *
     * @return string
     */
    static string getLine(const char** ppChar);

	/**
     * 读取一行
     * @param ppChar
     * @param iBufLen
     *
     * @return string
     */
    static string getLine(const char** ppChar, int iBufLen);

    /**
     * 生成头部字符串(不包含第一行)
     *
     * @return string
     */
    string genHeader() const;

    /**
     * 解析请求head(不解析第一行,第一行请求包和响应包不一样)
     * 后面的数据解析为map格式
     * @param szBuffer
     *
     * @return const char*, 偏移的指针
     */
    static const char* parseHeader(const char* szBuffer, http_header_type &sHeader);

protected:

    /**
     * http头部所有变量
     */
    http_header_type    _headers;

    /**
     * http头部长度
     */
    size_t              _headLength;

    /**
     * http头部内容
     */
    string              _content;
};

/********************* TC_HttpCookie***********************/

/**
 * 简单的Cookie管理类
 */
class TC_HttpCookie
{
public:
    typedef map<string, string, TC_Http::CmpCase> http_cookie_data;

    struct Cookie
    {
        http_cookie_data    _data;
        string              _domain;
        string              _path;
        time_t              _expires;       //等于0表示只在当前回话有效
        bool                _isSecure;
    };

    /**
     * 清空数据
     */
    void clear();

    /**
     * 增加Cookie到管理器中
     * @param sCookieRspURL: 产生该Cookie HTTP响应的URL
     * @param vCookies: Set-Cookie字符串
     */
    void addCookie(const string &sCookieRspURL, const vector<string> &vCookies);

    /**
     * 增加Cookie到管理器
     * @param cookie
     */
    void addCookie(const Cookie &cookie);

    /**
     * 增加Cookie到管理器
     * @param cookie
     */
    void addCookie(const list<Cookie> &cookie);

    /**
     * 获取某个url的cookie域值对
     * 去掉了Domain,Path等字段
     * 
     * @param sReqURL: 需要匹配的URL
     * @param cookies
     */
    void getCookieForURL(const string &sReqURL, list<Cookie> &cookies);

    /**
     * 获取某个url的cookie域值对
     * 去掉了Domain,Path等字段
     * 
     * @param sReqURL: 需要匹配的URL
     * @param http_cookie_type
     * @param string
     */
    void getCookieForURL(const string &sReqURL, string &sCookie);

    /**
     * 匹配域名
     * sCookieDomain串有多个域
     * sCookieDomain未以.开头, 则sCookieDomain=sDomain
     * sCookieDomain以.开头,则sDomain右边匹配sDomain,sDomain比sCookieDomain以最多多一个.
     * (x.y.z.com 匹配 .y.z.com, x.y.z.com 不匹配.z.com )
     * 
     * @param sCookieDomain
     * @param sDomain
     * 
     * @return bool
     */
    static bool matchDomain(const string &sCookieDomain, const string &sDomain);

    /**
     * 匹配路径
     * sCookiePath是sPath的前缀
     * 
     * @param sCookiePath
     * @param sPath
     * 
     * @return size_t, 匹配路径的长度
     */
    static size_t matchPath(const string &sCookiePath, const string &sPath);

    /**
     * 获取所有的Cookie
     * 
     * @return list<Cookie>&
     */
    list<Cookie> getAllCookie();

    /**
     * 删除过期的Cookie
     * 仅仅存在与当前回话的Cookie不删除
     * @param t:超过当前时间的cookie都删除
     * @param bErase: true:存在于当前回话的Cookie也删除
     *                false:存在于当前回话的Cookie不删除
     */
    void deleteExpires(time_t t, bool bErase = false);

    /**
     * 返回所有没有过期的Cookie且有Expires字段的Cookie
     * 即仅仅存在与当前回话的Cookie是不返回的
     * 通常用于永久序列化
     * 
     * @param t:超过当前时间的cookie都删除
     * 
     * @return list<Cookie>
     */
    list<Cookie> getSerializeCookie(time_t t);

protected:
    /**
     * 判断Cookie是有效
     * @param tURL
     * 
     * @return size_t
     */
    size_t isCookieMatch(const Cookie &cookie, const TC_URL &tURL) const;

    /**
     * 检查Cookie是否过期
     * 当前回话的不算过期(expires=0)
     * @param cookie
     * 
     * @return bool
     */
    bool isCookieExpires(const Cookie &cookie) const;

    /**
     * 添加Cookie
     * @param cookie
     * @param cookies
     */
    void addCookie(const Cookie &cookie, list<Cookie> &cookies);

    /**
     * 修正Domain
     * @param sDomain
     * 
     * @return string
     */
    bool fixDomain(const string &sDomain, string &sFixDomain);

protected:

    /**
     * 保存Cookie
     * key:domain+path
     */
    list<Cookie> _cookies;
};

/********************* TC_HttpResponse ***********************/

class TC_HttpResponse : public TC_Http
{
public:

    /**
     * 构造
     */
    TC_HttpResponse()
    {
        TC_HttpResponse::reset();
    }

    /**
     * reset响应包
     */
    void reset();

    /**
     * 增量decode,输入的buffer会自动在解析过程中被清除掉
     * 增量decode之前必须reset
     * (网络接收的buffer直接添加到sBuffer里面即可, 然后增量解析)
     * (能够解析的数据TC_HttpResponse会自动从sBuffer里面消除，直到网络接收完毕或者解析返回true)
     * @param buffer
     * @throws TC_HttpResponse_Exception, 不支持的http协议, 抛出异常
     * @return bool,
     *      true:解析出一个完整的buffer
     *      false:还需要继续解析(如果服务器主动关闭连接的模式下, 如果服务器主动关闭连接的模式下, 也可能不需要再解析了)
     *      )
     * 
     */
    bool incrementDecode(string &sBuffer);

    /**
     * 解析http应答(采用string方式)
     * 注意:如果http头部没有Content-Length且非chunk模式, 则返回true
     * 需要网络层判断(http服务端主动关闭连接也算是发送http响应完整了)
     * @param sBuffer
     * @return bool, sBuffer是否是完整的http请求
     */
    bool decode(const string &sBuffer);

    /**
     * 解析http应答
     * 注意:如果http头部没有Content-Length且非chunk模式, 则返回true
     * 需要网络层判断(http服务端主动关闭连接也算是发送http响应完整了)
     * @param sBuffer
     * @return bool, sBuffer是否是完整的http请求
     */
    bool decode(const char *sBuffer, size_t iLength);

    /**
     * 编码应答包(采用string方式)
     * @param sBuffer
     *
     * @return string
     */
    string encode() const;

    /**
     * 编码应答包(采用vector<char>方式)
     * @param sBuffer
     *
     * @return string
     */
    void encode(vector<char> &sBuffer) const;

    /**
     * 获取第一行(HTTP/1.1 200 OK)
     * 
     * @return string
     */
    string getResponseHeaderLine() const { return _headerLine; }

    /**
     * 获取HTTP响应状态(例如200)
     *
     * @return int
     */
    int  getStatus() const {return _status; }

    /**
     * 设置状态
     * @param status
     */
    void setStatus(int status) { _status = status; }

    /**
     * 描述(例如OK)
     * 
     * @return string
     */
    string getAbout() const { return _about; }

    /**
     * 设置描述
     * @param about
     */
    void setAbout(const string &about) { _about = about; }

    /**
     * 获取版本, 例如HTTP/1.1
     * 
     * @return string
     */
    string getVersion() const { return _version; }

    /**
     * 另种模式:HTTP/1.1(默认)或者HTTP/1.0
     * @param sVersion
     */
    void setVersion(const string &sVersion) { _version = sVersion; }

    /**
     * 设置应答状态(采用string方式)
     * @param status
     * @param about
     * @param sBody, post协议body的内容
     */
    void setResponse(int status = 200, const string& about = "OK", const string& sBody = "");

    /**
     * 设置应答状态
     * @param status
     * @param about
     * @param sBuffer, post协议body的内容
     * @param iLength, sBuffer长度
     */
    void setResponse(int status, const string& about, const char *sBuffer, size_t iLength);

    /**
     * 设置应答, 缺省status=200, about=OK
     * @param sBuffer
     * @param iLength
     */
    void setResponse(const char *sBuffer, size_t iLength);

    /**
     * 设置服务
     * @param sServer
     */
    void setServer(const string &sServer){setHeader("Server", sServer);}

    /**
     * 设置Set-Cookie
     * @param sCookie
     */
    void setSetCookie(const string &sCookie){setHeader("Set-Cookie", sCookie);}

    /**
     * 获取Set-Cookie
     * 
     * @return vector<string>
     */
    vector<string> getSetCookie() const;

    /**
     * 解析应答头
     * @param szBuffer
     *
     * @return
     */
    void parseResponseHeader(const char* szBuffer);

protected:

    /**
     * 应答状态
     */
    int     _status;

    /**
     * 应答描述
     */
    string  _about;

    /**
     * 获取版本
     */
    string  _version;

    /**
     * 获取第一行
     */
    string  _headerLine;

    /**
     * 临时的content length
     */
    size_t  _iTmpContentLength;
};

/********************* TC_HttpRequest ***********************/

class TC_HttpRequest : public TC_Http
{
public:

    ///////////////////////////////////////////////////////////////////
    TC_HttpRequest()
    {
        TC_HttpRequest::reset();
        setUserAgent("TC_Http");
    }

    /**
     * 请求协议类型
     */
    enum
    {
        REQUEST_GET,
        REQUEST_POST
    };

    /**
     * 检查http请求是否收全
     * @param sBuffer
     * @throws TC_HttpRequest_Exception, 不支持的http协议, 抛出异常
     * @return bool, true: 收全, false:不全
     */
    static bool checkRequest(const char* sBuffer, size_t len);

    /**
     * 重置
     */
    void reset();

    /**
     * 设置 User-Agent
     * @param sUserAgent
     */
    void setUserAgent(const string &sUserAgent){setHeader("User-Agent", sUserAgent);}

    /**
     * 设置 Cookie
     * @param sCookie
     */
    void setCookie(const string &sCookie){setHeader("Cookie", sCookie);}

    /**
     * 获取原始Cookie行
     * 
     * @return vector<string>
     */
    vector<string> getCookie();

    /**
     * 解析http请求, 如果不是HTTP请求则抛出异常
     * @param sBuffer
     * @throw TC_HttpRequest_Exception
     * @return bool, sBuffer是否是完整的http请求
     */
    bool decode(const string &sBuffer);

    /**
     * 解析http请求, 如果不是HTTP请求则抛出异常(采用vector<char>方式)
     * @param sBuffer
     * @param iLength: 长度
     * @throw TC_HttpRequest_Exception
     * @return bool, sBuffer是否是完整的http请求
     */
    bool decode(const char *sBuffer, size_t iLength);

    /**
     * 生成请求(采用string方式)
     * @param sUrl
     *
     * @return string
     */
    string encode();

    /**
     * 生成请求(采用vector<char>方式)
     * @param buffer
     */
    void encode(vector<char> &buffer);

    /**
     * 设置Get请求包
     * @param sUrl, 例如:http://www.qq.com/query?a=b&c=d
     * @param bCreateHost, 是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     */
    void setGetRequest(const string &sUrl, bool bNewCreateHost = false);

    /**
     * 设置POST请求包(采用string方式)
     * @param sUrl, 例如:http://www.qq.com/query
     * @param sPostBody, 如果requestType是GET, 则sPostBody无效
     * @param bCreateHost, 是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     */
    void setPostRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost = false);

    /**
     * 设置POST请求包(采用vector<char>方式)
     * @param sUrl, 例如:http://www.qq.com/query
     * @param sPostBody, 如果requestType是GET, 则sPostBody无效
     * @param bCreateHost, 是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     */
    void setPostRequest(const string &sUrl, const char *sBuffer, size_t iLength, bool bNewCreateHost = false);

    /**
     * 获取url里面的地址和端口
     * @param sHost
     * @param iPort
     */
    void getHostPort(string &sDomain, uint32_t &iPort);

    /**
     * 发送HTTP请求
     * @param iTimeout, 毫秒
     *
     * @return int 0 成功, <0失败, 具体值参见TC_ClientSocket声明
     */
    int doRequest(TC_HttpResponse &stHttpRep, int iTimeout = 3000);

    /**
     * 是否是GET请求
     *
     * @return bool
     */
    bool isGET() const { return _requestType == REQUEST_GET; }

    /**
     * 是否是POST请求
     *
     * @return bool
     */
    bool isPOST() const { return _requestType == REQUEST_POST; }

    /**
     * 获取请求的URL
     * 
     * @return const TC_URL&
     */
    const TC_URL &getURL() const { return _httpURL; }

    /**
     * 获取完整的http请求
     * 
     * @return string
     */
    string getOriginRequest() const { return _httpURL.getURL(); }

    /**
     * 获取http请求的url
     * 不包括Host, 例如http://www.qq.com/abc?a=b, 则为:/abc?a=b
     *
     * @return string
     */
    string getRequest() const { return _httpURL.getRequest(); }

    /**
     * 获取http请求的url部分, 即?前面
     * 
     * 不包括Host, 例如http://www.qq.com/abc?a=b#def, 则为:/abc
     *
     * @return string
     */
    string getRequestUrl() const { return _httpURL.getPath(); }

    /**
     * 获取http请求url后的参数部分, 即?后面#前面部分
     * 
     * 不包括Host, 例如http://www.qq.com/abc?a=b#def, 则为:a=b
     *
     * @return string
     */
    string getRequestParam() const { return _httpURL.getQuery(); }

    /**
     * 解析请求头部
     * @param szBuffer
     *
     * @return size_t
     */
    size_t parseRequestHeader(const char* szBuffer);

    /**
     * 请求类型到字符串
     * @param iRequestType
     *
     * @return string
     */
    string requestType2str(int iRequestType) const;

protected:

    /**
     * 对http请求编码
     * @param sUrl
     * @param iRequestType
     *
     * @return void
     */
    void encode(int iRequestType, ostream &os);

    /**
     * 解析URL
     */
    void parseURL(const string& sUrl);

protected:

    /**
     * 请求URL
     */
    TC_URL             _httpURL;

    /**
     * 请求类型
     */
    int                _requestType;
};


}
#endif

