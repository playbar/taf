#ifndef __TC_OPTION_H
#define __TC_OPTION_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: 命令行参数解析类
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
 * 分析输入参数, 支持以下形式的参数
 * ./main.exe --name=value --with abc def
 */
class TC_Option
{
public:
    /**
     * 构造函数
     */
    TC_Option(){};

    /**
     * 解码
     * @param argc
     * @param argv
     *
     */
    void decode(int argc, char *argv[]);

    /**
     * 是否存在某个--标识的参数
     * @param sName
     *
     * @return bool
     */
    bool hasParam(const string &sName);

    /**
     * 获取某个--表示的参数, 如果参数不存在或者参数值为空, 都返回""
     * @param sName
     *
     * @return string
     */
    string getValue(const string &sName);

    /**
     * 获取所有--标识的参数
     *
     * @return map<string,string>
     */
    map<string, string>& getMulti();

    /**
     * 获取所有普通的参数, 例子中的abc, def
     * 参数按照顺序在vector中
     *
     * @return vector<string>
     */
    vector<string>& getSingle();

protected:

    /**
     *
     * @param s
     */
    void parse(const string &s);

protected:
    /**
     *
     */
    map<string, string> _mParam;

    /**
     *
     */
    vector<string>      _vSingle;
};

}

#endif

