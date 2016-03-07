#ifndef __TC_CONFIG_H_
#define __TC_CONFIG_H_

#include <map>
#include <list>
#include <stack>
#include <vector>
#include "util/tc_ex.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 配置文件读取类
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
* 域分隔符
*/
const char TC_CONFIG_DOMAIN_SEP = '/';

/**
* 参数开始符
*/
const char TC_CONFIG_PARAM_BEGIN = '<';

/**
* 参数结束符
*/
const char TC_CONFIG_PARAM_END = '>';

/**
* 配置文件异常类
*/
struct TC_Config_Exception : public TC_Exception
{
	TC_Config_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_Config_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
	~TC_Config_Exception() throw(){};
};

/**
 * 配置没有文件错误
 */
struct TC_ConfigNoParam_Exception : public TC_Exception
{
	TC_ConfigNoParam_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_ConfigNoParam_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
	~TC_ConfigNoParam_Exception() throw(){};
};


/**
* 定义配置文件中的域
*/
class TC_ConfigDomain
{
public:
    friend class TC_Config;

    /**
    * 构造函数
    * @param sLine : 行
    */
    TC_ConfigDomain(const string &sLine);

    /**
    * 析够函数
    */
    ~TC_ConfigDomain();

    /**
     * 拷贝复制
     * @param tcd
     */
    TC_ConfigDomain(const TC_ConfigDomain &tcd);

    /**
     * 赋值
     * @param tcd
     * 
     * @return TC_ConfigDomain&
     */
    TC_ConfigDomain& operator=(const TC_ConfigDomain &tcd);

    struct DomainPath
    {
        vector<string>  _domains;
        string          _param;
    };

    /**
     * 解析domain
     * @param domain
     *
     * @return bool
     */
    static DomainPath parseDomainName(const string& path, bool bWithParam);

    /**
    * 增加子域名称
    * @param pTcConfigDomain: pointer to sub domain
    * @throws TC_Config_Exception
    * @return TC_ConfigDomain*
    */
    TC_ConfigDomain* addSubDomain(const string& name);

    /**
    * Search Sub Domain 递归搜索
    * @param sDomainName : /domain1/domain2
    * @return pointer to sub domain
    */
    TC_ConfigDomain *getSubTcConfigDomain(vector<string>::const_iterator itBegin, vector<string>::const_iterator itEnd);
    const TC_ConfigDomain *getSubTcConfigDomain(vector<string>::const_iterator itBegin, vector<string>::const_iterator itEnd) const;

    /**
    * Get Param Value 获取参数/值对
    * @param sName : 名称
    * @return 值
    */
    string getParamValue(const string &sName) const;

    /**
    * Get Param Map 获取map
    * @return map
    */
    const map<string, string>& getParamMap() const { return _param; }

    /**
    * Get Domain Map 获取map
    * @return map
    */
    const map<string, TC_ConfigDomain*>& getDomainMap() const { return _subdomain; }

    /**
    * Set Param Value 设置参数/值对
    * @param sLine : 行
    * @return 无
    */
    void setParamValue(const string &sLine);

    /**
     * 插入参数
     * @param m
     */
    void insertParamValue(const map<string, string> &m);

    /**
    * Destroy 释放
    * @return 无
    */
    void destroy();

    /**
    * Get Name 获取域名称
    * @return 域名称
    */
    string getName() const;

    /**
     * 设置域名称
     * @param name
     */
    void setName(const string& name);

    /**
     * 按照文件中的顺序获取key
     * 
     * @return vector<string>
     */
    vector<string> getKey() const;

    /**
     * 按照文件中的顺序获取子Domain
     * 
     * @return vector<string>
     */
    vector<string> getSubDomain() const;

    /**
    * tostr 转换成配置文件的字符串格式
    * @param i : tab的层数
    * @return 域名称
    */
    string tostr(int i) const;

    /**
     * 克隆
     * 
     * @return TC_ConfigDomain*
     */
    TC_ConfigDomain* clone() const 
    { 
        return new TC_ConfigDomain(*this); 
    }

protected:
    /**
     * 转义
     * @param name
     *
     * @return string
     */
    static string parse(const string& s);

    /**
     * 方向转义
     * @param s
     *
     * @return string
     */
    static string reverse_parse(const string &s);

    /**
    * Set Param Value 设置参数/值对
    * @param sName : 名称
    * @param sValue : 值
    * @return 无
    */
    void setParamValue(const string &sName, const string &sValue);

protected:

    /**
    * 域名称
    */
    string                  _name;

    /**
    * name value对
    */
    map<string, string>     _param;

    /**
     * key的插入顺序
     */
    vector<string>          _key;

    /**
    * 子域
    */
    map<string, TC_ConfigDomain*>	    _subdomain;

    /**
     * 域的插入顺序
     */
    vector<string>          _domain;
};

/**
* 定义配置文件
* <Main>
*   <Domain>
*       Name = Value
*   </Domain>
*   <Domain1>
*       Name = Value
*   </Domain1>
* </Main>
*
* 获取参数:conf["/Main/Domain<Name>"]
* 获取域Map: getDomainMap("/Main/Domain", m); m得到NameValue对
* 获取域Vector: getDomainVector("/Main", v); v得到Domain列表
* 可以增加域或域下面的值对
*/
class TC_Config
{
public:

    /**
    * contructor
    */
    TC_Config();

    /**
     * 拷贝复制
     * @param tc
     */
    TC_Config(const TC_Config &tc);

    /**
     * 赋值
     * @param tcd
     * 
     * @return TC_Config&
     */
    TC_Config& operator=(const TC_Config &tc);

    /**
    * Parse File 解析文件
    * @param sFileName : 文件名称
    * @throws TC_Config_Exception
    * @return 无
    */
    void parseFile(const string& sFileName);

    /**
    * Parse 字符串
    * @throws TC_Config_Exception
    * @return void
    */
    void parseString(const string& buffer);

    /**
    * Get Value 获取值, 如果没有则抛出异常
    * @param sName : 参数名称,  例如: /Main/Domain<Param>
    * @throws TC_Config_Exception
    * @return string
    */
    string operator[](const string &sName);

    /**
     * Get Value 获取值, 注意如果没有不抛出异常,返回空字符串
     * @param sName
     *
     * @return string
     */
    string get(const string &sName, const string &sDefault="") const;

    /**
    * GetDomainParamMap 获取域下面的参数值对
    * @param path : 域名称, 域标识, 例如: /Main/Domain
    * @param m, 域内的map列表
    * @param map<string, string>
    * @return bool, 返回域下面的参数值对
    */
    bool getDomainMap(const string &path, map<string, string> &m) const;

    /**
     * 不存则返回空map
     * @param path: 域名称, 域标识, 例如: /Main/Domain
     * 
     * @return map<string,string>
     */
    map<string, string> getDomainMap(const string &path) const;

    /**
     * 获取域下面的所有key, 按照文件的行顺序返回
     * @param path: 域名称, 域标识, 例如: /Main/Domain
     * 
     * @return vector<string>
     */
    vector<string> getDomainKey(const string &path) const;

    /**
    * getDomainMap 获取域下面的子域
    * @param path : 域名称, 域标识, 例如: /Main/Domain
    * @param v : 域下面的域名称,
    * @param vector<string>, 返回域下面的域名称
    * @return bool
    */
    bool getDomainVector(const string &path, vector<string> &v) const;

    /**
     * 不存在则返回空vector
     * 按照在文件中的顺序
     * @param path: 域名称, 域标识, 例如: /Main/Domain
     * 
     * @return vector<string>
     */
    vector<string> getDomainVector(const string &path) const;

    /**
     * 是否存在域
     * @param path: 域名称, 域标识, 例如: /Main/Domain
     *
     * @return bool
     */
    bool hasDomainVector(const string &path) const;

    /**
     * 当前域下面增加域, 如果已经存在sAddDomain域, 则认为成功
     * @param sCurDomain, 域标识符, 例如:/Main/Domain
     * @param sAddDomain, 需要增加的域名称: 例如: Domain1
     * @param bCreate, sCurDomain域不存在的情况下, 是否自动创建
     * @return int, 0:成功
     *              -1:sCurDomain不存在
     */
    int insertDomain(const string &sCurDomain, const string &sAddDomain, bool bCreate);

    /**
     * 当前域下面增加参数, 如果已经有相关参数, 则忽略(不替换)
     * @param sCurDomain, 域标识符, 例如:/Main/Domain
     * @param m,参数值对
     * @param bCreate, sCurDomain域不存在的情况下, 是否自动创建
     * @return int, 0: 成功
     *              -1:sCurDomain不存在
     */
    int insertDomainParam(const string &sCurDomain, const map<string, string> &m, bool bCreate);

    /**
     * 合并配置文件到当前配置文件
     * @param cf
     * @param bUpdate, true: 冲突项更新本配置, false: 冲突项不更新
     */
    void joinConfig(const TC_Config &cf, bool bUpdate);

    /**
    * 转换成配置文件的字符串格式
    * @return 配置字符串
    */
	string tostr() const;

protected:
    /**
    * Parse 字符串
    * @throws TC_Config_Exception
    * @return void
    */
    void parse(istream &is);

    /**
    * create New Domain 生成子域
    * @param sName : 域名称
    * @throws TC_Config_Exception
    * @return pointer to new domain
    */
    TC_ConfigDomain *newTcConfigDomain(const string& sName);

    /**
    * Search Domain 搜索域
    * @param sDomainName 参数名称,支持子域搜索
    * @return value
    */
	TC_ConfigDomain *searchTcConfigDomain(const vector<string>& domains);
	const TC_ConfigDomain *searchTcConfigDomain(const vector<string>& domains) const;

protected:

    /**
    * 根domain
    */
	TC_ConfigDomain _root;
};

}
#endif //_TC_CONFIG_H_
