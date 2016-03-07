#ifndef __TC_CGI_H
#define __TC_CGI_H

#include <sstream>
#include <istream>
#include <map>
#include <vector>
#include "util/tc_ex.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: CGI处理类
// Author : j@syswin.com              
/////////////////////////////////////////////////
class TC_Cgi;
class TC_Cgi_Upload;
class TC_HttpRequest;

/**
* 配置文件异常类
*/
struct TC_Cgi_Exception : public TC_Exception
{
	TC_Cgi_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_Cgi_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
	~TC_Cgi_Exception() throw(){};
};

/**
* 全局的友元函数
* 定义该函数,则可以用TC_Common::tostr对 vector<TC_Cgi_Upload>进行输出操作
*/
ostream &operator<<(ostream &os, const TC_Cgi_Upload &tcCgiUpload);

/**
* cgi上传文件操作
* 通过该类获取cgi上传的文件信息
*/
class TC_Cgi_Upload
{
public:
    friend ostream &operator<<(ostream &os, const TC_Cgi_Upload &tcCgiUpload);

    /**
    * contructor
    */
    TC_Cgi_Upload()
    :_sFileName("")
    , _sRealFileName("")
    , _sServerFileName("")
    , _iSize(0)
    , _bOverSize(false)
    {
    }

    /**
    * 拷贝构造函数 copy contructor
    * @param tcCgiUpload : 拷贝构造函数
    */

    TC_Cgi_Upload(const TC_Cgi_Upload &tcCgiUpload);

    /**
    * 赋值构造函数
    */
    TC_Cgi_Upload & operator=(const TC_Cgi_Upload &tcCgiUpload);

    /**
    * 返回上传的信息
    * return string : 上传文件的信息
    */
    string tostr() const;

    /**
    * 返回客户端IE INPUT上传控件的名称
    * return string : INPUT上传控件名称
    */
    string getFormFileName() const
    {
        return _sFileName;
    }

    /**
    * 返回INPUT控件用户输入的名称,及客户端真实的文件名称
    * return string : 客户端真实的文件名称
    */
    string retRealFileName() const
    {
        return _sRealFileName;
    }

    /**
    * 上传到服务器后,服务器端文件名称
    * return string : 服务器端文件名称
    */
    string getSeverFileName() const
    {
        return _sServerFileName;
    }

    /**
    * 上传的文件大小
    * return size_t : 上传的文件大小
    */
    size_t getFileSize() const
    {
        return _iSize;
    }

    /**
    * 上传的文件是否超过大小
    * return bool : 是否超过大小
    */
    bool isOverSize() const
    {
        return _bOverSize;
    }

protected:

    /**
    * 上传文件,浏览器file控件名称
    */
    string  _sFileName;

    /**
    * 上传文件真实名称,客户端的文件名称
    */
    string  _sRealFileName;

    /**
    * 上传文件服务器端名称
    */
    string  _sServerFileName;

    /**
    * 上传文件大小,字节数
     */
    size_t  _iSize;

    /**
    * 上传文件是否超过大小
    */
    bool    _bOverSize;

    friend class TC_Cgi;
};

/**
* cgi操作相关类
* 1 支持参数解析
* 2 支持cookies解析
* 3 支持文件上传,设置上传文件的最大个数,文件的最大大小
* 4 上传文件时, 需要检查文件是否超过最大大小
* 5 上传文件时, 需要检查上传文件个数是否限制
*
* 说明:多个文件同时上传时,浏览器的file控件必须取不同name,否则将无法正确的上传文件
*
*
* 注意:
* 调用parseCgi解析标准输入
* 如果有文件上传需要调用setUpload, 并且需要在parseCgi之前调用
*
*/
class TC_Cgi
{
public:

    /**
    * TC_Cgi contructor
    */
    TC_Cgi();

    /**
    * decontructor
    */
    virtual ~TC_Cgi();

    /**
    * 定义环境变量
    */
    enum
    {
        ENM_SERVER_SOFTWARE,
        ENM_SERVER_NAME,
        ENM_GATEWAY_INTERFACE,
        ENM_SERVER_PROTOCOL,
        ENM_SERVER_PORT,
        ENM_REQUEST_METHOD,
        ENM_PATH_INFO,
        ENM_PATH_TRANSLATED,
        ENM_SCRIPT_NAME,
        ENM_HTTP_COOKIE,
        ENM_QUERY_STRING,
        ENM_REMOTE_HOST,
        ENM_REMOTE_ADDR,
        ENM_AUTH_TYPE,
        ENM_REMOTE_USER,
        ENM_REMOTE_IDENT,
        ENM_CONTENT_TYPE,
        ENM_CONTENT_LENGTH,
        ENM_HTTP_USER_AGENT
    };

    /**
     * 设置上传文件
     * @param sUploadFilePrefix, 文件前缀(包含路径), 如果有文件上传,则文件保存在以该前缀为名称的路径下
     *                           如果有多个文件上传,则文件名称以次在后面加"_序号"
     * @param iMaxUploadFiles, 最多上传文件个数,<0:没有限制
     * @param iUploadMaxSize, 每个文件上传的最大大小(字节)
     */
    void setUpload(const string &sUploadFilePrefix, int iMaxUploadFiles = 5, size_t iUploadMaxSize = 1024*1024*10, size_t iMaxContentLength = 1024*1024*10);

    /**
     * 解析cgi
     * @param is: 从标准输入解析
     */
    void parseCgi();

    /**
     * 直接从http请求解析
     * @param request
     */
    void parseCgi(TC_HttpRequest &request);

    /**
    * 获取cgi的url参数multimap
    * @return multimap<string, string>
    */
    const multimap<string, string> &getParamMap() const;

    /**
     * 获取cgi环境变量map
     *
     * @return map<string,string>
     */
    map<string, string> getEnvMap() const { return _env; }

    /**
    * 获取cgi的参数map, 将multimap转换成map返回, 对于一个参数名称对应多个参数值的情况, 只取其中一个值
    * @return map<string, string>
    */
    map<string, string> getParamMapEx() const;

    /**
    * 获取cookies的参数map
    * @return map<string, string>
    */
    const map<string, string> &getCookiesMap() const;

    /**
    * 获取cgi的某个参数
    * @param sName : 参数名称
    * @return string
    */
    string &operator[](const string &sName);

    /**
    * 获取cgi的某个参数
    * @param sName : 参数名称
    * @return string
    */
    string getValue(const string& sName) const;

    /**
    * 获取某一名称的参数的多个值,用于解析checkbox这类控件的值(一个参数名,多个参数值)
    * @param sName : 参数名称
    * @param vtValue : 该名称的参数值组成的vector
    * @return vector<string>, 该名称的参数值组成的vector
    */
    const vector<string> &getMultiValue(const string& sName, vector<string> &vtValue) const;

    /**
    * 获取cookie值
    * @param sName : cookie名称
    * @return string
    */
    string getCookie(const string& sName) const;

    /**
    * 设置cookie值
    * @param sName : cookie名称
    * @param sValue : cookie值
    * @param sExpires : 过期日期
    * @param sPath : cookie有效路径
    * @param sDomain : cookie有效域
    * @param bSecure : 是否安全(ssl时有效)
    * @return string, 返回字符串
    */
    string setCookie(const string &sName, const string &sValue, const string &sExpires="", const string &sPath="/", const string &sDomain = "", bool bSecure = false);

    /**
    * 参数链表是否为空
    * @return bool
    */
    bool isParamEmpty() const;

    /**
    * 参数是否存在
    * @param sName, 参数名称
    * @return bool
    */
    bool isParamExist(const string& sName) const;

    /**
    * 上传文件是否超过大小,多个文件上传时,只要有一个文件超过大小,则超过
    * @return bool
    */
    bool  isUploadOverSize() const;

    /**
    * 上传文件是否超过大小,多个文件上传时,只要有一个文件超过大小,则超过
    * @param vtUploads : 返回超过大小的文件名称(浏览器file控件的名称)
    * return bool
    */
    bool  isUploadOverSize(vector<TC_Cgi_Upload> &vtUploads) const;

    /**
     * 是否超过上传文件个数
     *
     * @return bool
     */
    bool isOverUploadFiles() const { return _bOverUploadFiles; }

    /**
    * 上传文件个数
    * @return size_t
    */
    size_t getUploadFilesCount() const;

    /**
    * 获取上传文件的map
    * @return map<string, TC_Cgi_Upload>
    */
    const map<string, TC_Cgi_Upload> &getUploadFilesMap() const;

    /**
    * 获取环境变量
    * @param iEnv : 枚举变量
    * @return string
    */
    string getCgiEnv(int iEnv);

    /**
    * 获取环境变量
    * @param sEnv : 环境变量名称
    * @return string
    */
    string getCgiEnv(const string& sEnv);

    /**
     * 设置环境变量
     * @param sName
     * @param sValue
     */
    void setCgiEnv(const string &sName, const string &sValue);

    /**
    * 返回html头:content-type
    * @param sHeader : 内容类型
    * @return 无
    */
    static string htmlHeader(const string &sHeader = "text/html");

    /**
    * http请求的url解码, %后面的换成字符
    * @param sUrl : http请求url
    * @return string : 返回解码后的字符串
    */
    static string decodeURL(const string &sUrl);

    /**
     * 对url进行编码, 非数字和字母用%XX代替
     * @param sUrl
     *
     * @return string
     */
    static string encodeURL(const string &sUrl);

    /**
     * 对源字符串进行HTML编码(<>"&)
     * @param src
     * @param blankEncode, 是否对空格也编码(空格, \t, \r\n, \n)
     *
     * @return string
     */
    static string encodeHTML(const string &src, bool blankEncode = false);

    /**
     * 对源字符串进行XML编码(<>"&')
     * @param src
     *
     * @return string
     */
    static string encodeXML(const string &src);

protected:

    /**
    * 申明,但是不定义,保证这个函数不会被使用
    */
    TC_Cgi &operator=(const TC_Cgi &tcCgi);

    /**
    * GET method
    * @param sBuffer : GET的QueryString
    * return 无
    */
    void getGET(string &sBuffer);

    /**
    * POST method
    * @param sBuffer : POST的QueryString
    * return 无
    */
    void getPOST(string &sBuffer);

    /**
    * 解析文件上传
    * @param mmpParams : [out]输出参数multimap
    * return 无
    */
    void parseUpload(multimap<string, string> &mmpParams);

    /**
     * 解析form数据
     */
    void parseFormData(multimap<string, string> &mmpParams, const string &sBoundary);

    /**
     * 忽略空行
     */
    void ignoreLine();

    /**
     * 写文件
     * @param sFileName
     * @param sBuffer
     */
    bool writeFile(FILE*fp, const string &sFileName, const string &sBuffer, size_t &iTotalWrite);

    /**
    * 非上传模式下解析
    * @param mmpParams : [out]输出参数multimap
    * @param sBuffer : [in]输入QueryString
    * return 无
    */
    void parseNormal(multimap<string, string> &mmpParams, const string& sBuffer);

    /**
    * 解析cookies
    * @param mpCooies : [out]输出cookiesmap
    * @param sBuffer : [in]输入Cookies字符串
    * return 无
    */
    void parseCookies(map<string, string> &mpCooies, const string& sBuffer);

    /**
    * 控制解析cgi input的基本流程
    * @param mmpParams : [out]输出参数 multimap
    * @param mpCooies : [out]输出cookies
    * return 无
    */
    void readCgiInput(multimap<string, string> &mmpParams, map<string, string> &mpCooies);

protected:

    /**
     * buffer
     */
    string                      _buffer;

    /**
     * 流
     */
    istringstream               _iss;

    /**
     * 读入
     */
    istream                     *_is;

    /**
     * 环境变量
     */
    map<string, string>         _env;

    /**
    * cgi参数
    */
    multimap<string, string>    _mmpParams;

    /**
    * cookies
    */
    map<string, string>         _mpCookies;

    /**
    * 上传文件名称前缀
    */
    string                      _sUploadFilePrefix;

    /**
    * 上传文件的最大个数,<0:无限制
    */
    int                         _iMaxUploadFiles;

    /**
    * 上传文件的最大大小
    */
    size_t                      _iUploadMaxSize;

    /**
     * 是否超过上传文件个数
     */
    bool                        _bOverUploadFiles;

    /**
     * 最大的content-length
     */
    size_t                      _iMaxContentLength;

    /**
    * 是否超过大小,有一个文件超过则超过
    */
    bool                        _bUploadFileOverSize;

    /**
    * 上传文件相关信息保存在该map中
    */
    map<string, TC_Cgi_Upload>  _mpUpload;
};

}

#endif
