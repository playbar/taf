#ifndef _HELLO_IMP_H_
#define _HELLO_IMP_H_

#include "Hello.h"
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace Test;

class HelloImp : public Hello
{
public:
    /**
     *
     */
    HelloImp(){};

     /**
     * 初始化，Hello的虚拟函数，HelloImp初始化时调用
     *
     * @return int
     */
    virtual void initialize()
	{
		//注册处理函数：
		TAF_ADD_ADMIN_CMD_NORMAL("SETHELLO", HelloImp::procHello);

	}

    /**
     * 退出，Hello的虚拟函数，服务退出时调用
     */
	virtual void destroy(){};

    /**
     * testHello，实现jce文件中的接口
     *
     * @return int
     */
	int testHello(const string &s, string &r, JceCurrentPtr current)
	{
        DLOG << s << endl;
		r = s;
		sleep(10);
	    return 0;
	}

	int testHelloSlow(const string &s, string &r, JceCurrentPtr current)
	{
        LOG->debug() << "testHelloSlow::" << s << endl;
		r = s;
		sleep(10);
	    return 0;
	}

    int testHelloFast(const string &s, string &r, JceCurrentPtr current)
    {
        LOG->debug() << "testHelloFast::" << s << endl;

        r = s;
        
        return 0;
    }

protected:
	bool procHello(const string& command, const string& params, string& result)
	{
		_hello = params;
		return false;
	}

protected:
	string _hello;
};


#endif



