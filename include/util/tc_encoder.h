#ifndef __TC_ENCODER_H_
#define __TC_ENCODER_H_

#include <vector>

#include "util/tc_ex.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 转码类
// Author : j@syswin.com              
// Modify: j@syswin.com
/////////////////////////////////////////////////
/**
* 转码异常类
*/
struct TC_Encoder_Exception : public TC_Exception
{
	TC_Encoder_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_Encoder_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
	~TC_Encoder_Exception() throw(){};
};

/**
* 该类提供基本常用编码的转换
* 1 GBK ==> UTF8
* 2 UTF8 ==> GBK
*/
class TC_Encoder
{
public:
    /**
    * gbk 转换到 utf8
    * @param sOut : 输出buffer
    * @param iMaxOutLen : 输出buffer最大的长度/sOut的长度
    * @param sIn : 输入buffer
    * @param iInLen : 输入buffer长度
    * @throws TC_Encoder_Exception
    * @return 无
    */
    static void  gbk2utf8(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen);

    /**
    * gbk 转换到 utf8
    * @param sOut : 输出buffer
    * @param sIn : 输入buffer*
    * @throws TC_Encoder_Exception
    * @return string : utf8编码
    */
    static string gbk2utf8(const string &sIn);

    /**
    * gbk 转换到 utf8
    * @param sIn : 输入buffer
    * @param vtStr : 输出gbk的vector
    * @throws TC_Encoder_Exception
    * @return 无
    */
    static void gbk2utf8(const string &sIn, vector<string> &vtStr);

    /**
    * utf8 转换到 gbk
    * @param sOut : 输出buffer
    * @param iMaxOutLen : 输出buffer最大的长度/sOut的长度
    * @param sIn : 输入buffer
    * @param iInLen : 输入buffer长度
    * @throws TC_Encoder_Exception
    * @return 无
    */
    static void utf82gbk(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen);

    /**
    * utf8 转换到 gbk
    * @param sIn : 输入buffer
    * @throws TC_Encoder_Exception
    * @return string :
    */
    static string utf82gbk(const string &sIn);

	/**
	* 转义字符串中的某个字符
	* 缺省:\n 转换为 \r\0; \r 转换为 \r\r;
	* @param str: 待转换字符串
	* @param f: 需要转义的字符
	* @param t: 转义后的字符
	* @param u: 借用的转义符
	* @return str: 转换后的字符串
	*/
	static string transTo(const string& str, char f = '\n', char t = '\r', char u = '\0');

	/**
	* 将 transTo 的字符串还原
	* 缺省:\r\0 还原为\n; \r\r 还原为 \r;
	* @param str: 待还原的字符串(必须是transTo后得到的字符串)
	* @param f: 被转义的字符
	* @param t: 转义后的字符
	* @param u: 借用的转义符
	* @return str: 还原后的字符串
	*/
	static string transFrom(const string& str, char f = '\n', char t = '\r', char u = '\0');
};

}


#endif


