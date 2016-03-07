#ifndef __TC_COMMON_H
#define __TC_COMMON_H

#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif

#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <stack>
#include <vector>
#include "util/tc_loki.h"

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: 公用函数类
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
* 帮助类,都是静态函数,提供一些常用的函数
*/
class TC_Common
{
public:

    /**
    * 去掉头部以及尾部的字符或字符串
    * @param sStr: 输入字符串
    * @param s: 需要去掉的字符
    * @param bChar: 如果为true, 则去掉s中每个字符; 如果为false, 则去掉s字符串
    * @return string 返回去掉后的字符串
    */
    static string trim(const string &sStr, const string &s = " \r\n\t", bool bChar = true);

    /**
    * 去掉左边的字符或字符串
    * @param sStr: 输入字符串
    * @param s: 需要去掉的字符
    * @param bChar: 如果为true, 则去掉s中每个字符; 如果为false, 则去掉s字符串
    * @return string 返回去掉后的字符串
    */
    static string trimleft(const string &sStr, const string &s = " \r\n\t", bool bChar = true);

    /**
    * 去掉右边的字符或字符串
    * @param sStr: 输入字符串
    * @param s: 需要去掉的字符
    * @param bChar: 如果为true, 则去掉s中每个字符; 如果为false, 则去掉s字符串
    * @return string 返回去掉后的字符串
    */
    static string trimright(const string &sStr, const string &s = " \r\n\t", bool bChar = true);

    /**
    * 字符串转换成小写
    * @param sString: 字符串
    * @return string
    */
    static string lower(const string &sString);

    /**
    * 字符串转换成大写
    * @param sString: 字符串
    * @return string: 大写的字符串
    */
    static string upper(const string &sString);

    /**
    * 字符串是否都是数字的
    * @param sString: 字符串
    * @return bool: 是否是数字
    */
    static bool isdigit(const string &sInput);

    /**
    * 字符串转换成时间结构
    * @param sString: 字符串时间格式
    * @param sFormat: 格式
    * @param stTm: 时间结构
    * @return 0: 成功, -1:失败
    */
    static int str2tm(const string &sString, const string &sFormat, struct tm &stTm);

    /**
     * GMT格式的时间转化为时间结构(Sat, 06 Feb 2010 09:29:29 GMT, %a, %d %b %Y %H:%M:%S GMT)
     * 可以用mktime换成time_t, 但是注意时区
     * 可以用mktime(&stTm) - timezone换成本地的秒(time(NULL)值相同)
     * timezone是系统的, 需要extern long timezone;
     * @param sString
     * @param stTm
     * 
     * @return int, 0: 成功, -1:失败
     */
    static int strgmt2tm(const string &sString, struct tm &stTm);

    /**
    * 时间转换成字符串
    * @param stTm: 时间结构
    * @param sFormat: 格式
    * @return string 时间字符串
    */
    static string tm2str(const struct tm &stTm, const string &sFormat = "%Y%m%d%H%M%S");

    /**
    * 时间转换成字符串
    * @param t: 时间结构
    * @param sFormat: 格式
    * @return string 时间字符串
    */
    static string tm2str(const time_t &t, const string &sFormat = "%Y%m%d%H%M%S");

    /**
    * 当前时间转换成字符串,紧凑格式(%Y%m%d%H%M%S)
    * @param sFormat: 格式
    * @return string 时间字符串
    */
    static string now2str(const string &sFormat = "%Y%m%d%H%M%S");

    /**
    * 时间转换成GMT字符串,GMT格式:Fri, 12 Jan 2001 18:18:18 GMT
    * @param stTm: 时间结构
    * @return string 时间字符串
    */
    static string tm2GMTstr(const struct tm &stTm);

    /**
    * 时间转换成GMT字符串,GMT格式:Fri, 12 Jan 2001 18:18:18 GMT
    * @param stTm: 时间结构
    * @return string 时间字符串
    */
    static string tm2GMTstr(const time_t &t);

    /**
    * 当前时间转换成GMT字符串,GMT格式:Fri, 12 Jan 2001 18:18:18 GMT
    * @return string 时间字符串
    */
    static string now2GMTstr();

    /**
    * 当前的日期(年月日)转换成字符串(%Y%m%d)
    * @return string 时间字符串
    */
    static string nowdate2str();

    /**
    * 当前的时间(时分秒)转换成字符串(%H%M%S)
    * @return string 时间字符串
    */
    static string nowtime2str();

    /**
     * 当前的毫秒
     *
     * @return int64_t
     */
    static int64_t now2ms();

    /**
     * 取出当前微秒
     *
     * @return int64_t
     */
    static int64_t now2us();

    /**
    * 字符串转化成T型(如果T是数值类型, 如果str为空,则T为0)
    * @param sStr: 字符串
    * @return T , T型
    */
    template<typename T>
    static T strto(const string &sStr);

    /**
    * 字符串转化成T型
    * @param sStr: 字符串
    * @param sDefault, 缺省值
    * @return T , T型
    */
    template<typename T>
    static T strto(const string &sStr, const string &sDefault);

    /**
    * 解析字符串,用分隔符号分隔,保存在vector里
    * 例子: |a|b||c|
    *
    * 如果withEmpty=true时, 采用|分隔为:"","a", "b", "", "c", ""
    * 如果withEmpty=false时, 采用|分隔为:"a", "b", "c"
    * 如果T类型为int等数值类型, 则分隔的字符串为"", 则强制转化为0
    * 
    * @param sStr: 输入字符串
    * @param sSep: 分隔字符串(每个字符都算为分隔符)
    * @param withEmpty: true: 空的也算一个元素, false:空的过滤
    * @return vector<string>
    */
    template<typename T>
    static vector<T> sepstr(const string &sStr, const string &sSep, bool withEmpty = false);

    /**
    * T型转换成字符串,只要T能够使用ostream对象用<<重载,即可以被该函数支持
    * @param t:
    * @return string 字符串
    */
    template<typename T>
    static string tostr(const T &t);

    /**
     * vector转换成string
     * @param T
     * @param sSep, 后接的字符串
     *
     * @return string
     */
    template<typename T>
    static string tostr(const vector<T> &t);

    /**
     * map输出
     * @param K
     * @param V
     * @param t
     *
     * @return string
     */
    template<typename K, typename V, typename D, typename A>
    static string tostr(const map<K, V, D, A> &t);

    /**
     * map输出
     * @param K
     * @param V
     * @param t
     *
     * @return string
     */
    template<typename K, typename V, typename D, typename A>
    static string tostr(const multimap<K, V, D, A> &t);

    /**
    * pair 转化为字符串, 保证map等关系容器可以直接用tostr来输出
    * @param pair<F, S>: pair对象
    * @return string 字符串
    */
    template<typename F, typename S>
    static string tostr(const pair<F, S> &itPair);

    /**
    * container 转换成字符串
    * @param iFirst: 迭代器
    * @param iLast: 迭代器
    * @param sSep: 两个元素之间的分隔符
    * @return string 字符串
    */
    template <typename InputIter>
    static string tostr(InputIter iFirst, InputIter iLast, const string &sSep = "|");

    /**
    * 二进制数据转换成字符串
    * @param buf: 二进制buffer
    * @param len: buffer长度
    * @param sSep: 分隔符
    * @param lines: 多少个字节换一行, 默认0表示不换行
    * @return string 字符串
    */
    static string bin2str(const void *buf, size_t len, const string &sSep = "", size_t lines = 0);

    /**
    * 二进制数据转换成字符串
    * @param sBinData: 二进制数据
    * @param sSep: 分隔符
    * @param lines: 多少个字节换一行, 默认0表示不换行
    * @return string 字符串
    */
    static string bin2str(const string &sBinData, const string &sSep = "", size_t lines = 0);

    /**
    * 字符串转换成二进制
    * @param psAsciiData: 字符串
    * @param sBinData: 二进制数据
    * @param iBinSize: 需要转换的字符串长度
    * @return int 转换后的长度
    */
    static int str2bin(const char *psAsciiData, unsigned char *sBinData, int iBinSize);

    /**
     * 字符串转换成二进制
     * @param sBinData
     * @param sSep
     * @param lines
     *
     * @return string
     */
    static string str2bin(const string &sBinData, const string &sSep = "", size_t lines = 0);

    /**
    * 替换字符串
    * @param sString: 输入字符串
    * @param sSrc: 原字符串
    * @param sDest: 目的字符串
    * @return string: 替换后的字符串
    */
    static string replace(const string &sString, const string &sSrc, const string &sDest);

    /**
     * 匹配以.分隔的字符串, pat中*则代表通配符:代表非空的任何字符串
     * s为空, 返回false
     * pat为空, 返回true
     * s为普通字符串
     * pat为带*则被匹配的字符串
     * 用来匹配ip地址
     * @param s
     * @param pat
     *
     * @return bool
     */
    static bool matchPeriod(const string& s, const string& pat);
    
     /**
     * @param s
     * @param pat
     *
     * @return bool
     */
    static bool matchPeriod(const string& s, const vector<string>& pat);

    /**
     * 判断一个数是否是素数
     * @param n
     *
     * @return bool
     */
    static bool isPrimeNumber(size_t n);

    /**
     * daemon
     */
    static void daemon();

    /**
     * 忽略管道异常
     */
    static void ignorePipe();

    /**
     * %xx转成一个字节
     * @param sWhat
     *
     * @return char
     */
    static char x2c(const string &sWhat);

    /**
     * 大小字符串换成字节数, 支持K, M, G两种
     * 例如: 1K, 3M, 4G, 4.5M, 2.3G
     * @param s
     * @param iDefaultSize, 缺省的大小, 格式错误时, 缺省的大小
     * @return size_t
     */
    static size_t toSize(const string &s, size_t iDefaultSize);
};

namespace p
{
    template<typename D>
    struct strto1
    {
        D operator()(const string &sStr)
        {
            string s = "0";

            if(!sStr.empty())
            {
                s = sStr;
            }

            istringstream sBuffer(s);

            D t;
            sBuffer >> t;

            return t;
        }
    };

	template<>
    struct strto1<char>
    {
        char operator()(const string &sStr)
        {
            if(!sStr.empty())
			{
				return sStr[0];
			}
			return 0;
        }
    };

    template<typename D>
    struct strto2
    {
        D operator()(const string &sStr)
        {
            istringstream sBuffer(sStr);

            D t;
            sBuffer >> t;

            return t;
        }
    };

    template<>
    struct strto2<string>
    {
        string operator()(const string &sStr)
        {
            return sStr;
        }
    };
	
}

template<typename T>
T TC_Common::strto(const string &sStr)
{
    typedef typename TL::TypeSelect<TL::TypeTraits<T>::isStdArith, p::strto1<T>, p::strto2<T> >::Result strto_type;

    return strto_type()(sStr);
}

template<typename T>
T TC_Common::strto(const string &sStr, const string &sDefault)
{
    string s;

    if(!sStr.empty())
    {
        s = sStr;
    }
    else
    {
        s = sDefault;
    }

    istringstream sBuffer(s);

    T t;
    sBuffer >> t;

    return t;
}


template<typename T>
vector<T> TC_Common::sepstr(const string &sStr, const string &sSep, bool withEmpty)
{
    vector<T> vt;

    string::size_type pos = 0;
    string::size_type pos1 = 0;

    while(true)
    {
        string s;
        pos1 = sStr.find_first_of(sSep, pos);
        if(pos1 == string::npos)
        {
            if(pos + 1 <= sStr.length())
            {
                s = sStr.substr(pos);
            }
        }
        else if(pos1 == pos)
        {
            s = "";
        }
        else
        {
            s = sStr.substr(pos, pos1 - pos);
            pos = pos1;
        }

        if(withEmpty)
        {
            vt.push_back(strto<T>(s));
        }
        else
        {
            if(!s.empty())
            {
                T tmp = strto<T>(s);
                vt.push_back(tmp);
            }
        }

        if(pos1 == string::npos)
        {
            break;
        }

        pos++;
    }

    return vt;
}

template<typename T>
string TC_Common::tostr(const T &t)
{
    ostringstream sBuffer;
    sBuffer << t;
    return sBuffer.str();
}

template<typename T>
string TC_Common::tostr(const vector<T> &t)
{
    ostringstream sBuffer;
    for(size_t i = 0; i < t.size(); i++)
    {
        sBuffer << tostr(t[i]) << " ";
    }
    return sBuffer.str();
}

template<typename K, typename V, typename D, typename A>
string TC_Common::tostr(const map<K, V, D, A> &t)
{
    ostringstream sBuffer;
    typename map<K, V, D, A>::const_iterator it = t.begin();
    while(it != t.end())
    {
        sBuffer << " [" << tostr(it->first) << "]=[" << tostr(it->second) << "] ";
        ++it;
    }
    return sBuffer.str();
}

template<typename K, typename V, typename D, typename A>
string TC_Common::tostr(const multimap<K, V, D, A> &t)
{
    ostringstream sBuffer;
    typename multimap<K, V, D, A>::const_iterator it = t.begin();
    while(it != t.end())
    {
        sBuffer << " [" << tostr(it->first) << "]=[" << tostr(it->second) << "] ";
        ++it;
    }
    return sBuffer.str();
}

template<typename F, typename S>
string TC_Common::tostr(const pair<F, S> &itPair)
{
    ostringstream sBuffer;
    sBuffer << "[" << (itPair.first) << "]=[" << (itPair.second) << "]";
    return sBuffer.str();
}

template <typename InputIter>
string TC_Common::tostr(InputIter iFirst, InputIter iLast, const string &sSep)
{
    string sBuffer;
    InputIter it = iFirst;

    while(it != iLast)
    {
        sBuffer += tostr(*it);
        ++it;

        if(it != iLast)
        {
            sBuffer += sSep;
        }
        else
        {
            break;
        }
    }

    return sBuffer;
}

}
#endif

