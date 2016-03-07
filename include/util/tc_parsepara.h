#ifndef __TC_PARSEPARA_H
#define __TC_PARSEPARA_H

#include <map>
#include <string>

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: name=value格式解析类(非标准的http协议)
// Author : j@syswin.com              
/////////////////////////////////////////////////
class TC_Parsepara
{
public:

	TC_Parsepara(){};

    /**
    * contructor
    * @param : name=value&name=value字符串
    */
	TC_Parsepara(const string &sParam);

    /**
    * copy contructor
    * @param : name=value&name=value字符串
    */
	TC_Parsepara(const map<string, string> &mpParam);

    /**
    * copy contructor
    */
	TC_Parsepara(const TC_Parsepara &para);

    /**
    * =
    */
	TC_Parsepara &operator=(const TC_Parsepara &para);

    /**
    * ==
    */
	bool operator==(const TC_Parsepara &para);

    /**
    *+
    */
	const TC_Parsepara operator+(const TC_Parsepara &para);

    /**
    * +=
    */
	TC_Parsepara& operator+=(const TC_Parsepara &para);

    /**
    * decontructor
    */
	~TC_Parsepara();

    /**
    * 遍历函数
    * @param 参数名称
    * @param 参数值
    */
	typedef int (*TC_ParseparaTraverseFunc)(string, string, void *);

    /**
    *重载[], 获取参数值
    *@return string &参数值
    */
	string &operator[](const string &sName);

    /**
    * 读入字符串,并解析
    * @param sParam:字符串函数
    * @return 无
    */
	void load(const string &sParam);

    /**
    * 读入map,并解析
    * @param mpParam:字符串函数
    * @return void
    */
	void load(const map<string, string> &mpParam);

    /**
    * 转成字符串
    * @return string
    */
	string tostr() const;

    /**
    * 获取参数值
    * @param sName 参数名称
    * @return string
    */
	string getValue(const string &sName) const;

    /**
    * 设置参数值
    * @param sName 参数名称
    * @param sValue 参数值
    * @return void
    */
	void setValue(const string &sName, const string &sValue);

    /**
    * 清除当前参数值对
    * return void
    */
	void clear();

    /**
    * 引用方式获取参数map
    * @return map<string,string>& 返回参数map
    */
    map<string,string> &toMap();

    /**
    * 引用方式获取参数map
    * @return map<string,string>& 返回参数map
    */
    const map<string,string> &toMap() const;

    /**
    * 遍历每个参数值对
    * @param func: 函数
    * @param pParam: 参数,传入func中
    * @return void
    */
	void traverse(TC_ParseparaTraverseFunc func, void *pParam);

    /**
    * 对字符串解码,%XX转成字符,类似于http协议的编码
    * @param sParam 参数
    * @return string,解码后的字符串
    */
	static string decodestr(const string &sParam);

    /**
    * 对字符串编码,特殊字符转成%XX,类似于http协议的编码(少了对空格=>+的特殊处理)
    * @param sParam 参数
    * @return string, 编码后的字符串
    */
	static string encodestr(const string &sParam);

protected:

    /**
    * 字符串转换成map
    * @param sParam 参数
    * @param mpParam map
    * @return void
    */
	void decodeMap(const string &sParam, map<string, string> &mpParam) const;

    /**
    * map转换成字符串
    * @param mpParam map
    * @return string, 转换后的字符串
    */
	string encodeMap(const map<string, string> &mpParam) const;

    /**
    * "%xx" 转换为字符
    * @param sWhat: %xx字符串后面的两个字符
    * @return char 返回字符
    */
	static char x2c(const string &sWhat);

protected:

	map<string, string> _param;
};

}
#endif /*_TC_PARSEPARA_H*/
