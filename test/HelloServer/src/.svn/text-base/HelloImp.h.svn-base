#ifndef _HELLO_IMP_H_
#define _HELLO_IMP_H_

#include "Hello.h"
#include <sys/syscall.h>
#include <unistd.h>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace taf;
using namespace Test;

/////////////////////////////////////////////////////////////////////////
class HelloImp : public Servant
{

public:
    /**
     * 构造函数
     */
    HelloImp();

public:
    /**
     * 对象初始化
     */
    virtual void initialize();

    /**
     * 处理客户端的主动请求
     * @param current 
     * @param response 
     * @return int 
     */
    virtual int doRequest(taf::JceCurrentPtr current, vector<char>& response);

    /**
     * @param resp 
     * @return int 
     */
    virtual int doResponse(ReqMessagePtr resp);

    /**
     * 对象销毁
     */
    virtual void destroy();

public:
    /**
     * @param s 
     * @param r 
     * @param current 
     * @return int 
     */
    int testHello(const string &s, string &r, JceCurrentPtr current);

    /**
     * @param command 
     * @param params 
     * @param result 
     * @return bool 
     */
    bool procHello(const string& command, const string& params, string& result);
};
///////////////////////////////////////////////////////////////////////////////
#endif
