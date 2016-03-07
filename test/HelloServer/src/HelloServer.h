#ifndef _HelloServer_H_
#define _HelloServer_H_

#include <iostream>
#include "util/tc_http.h"
#include "util/tc_cgi.h"
#include "servant/Application.h"

using namespace taf;

class HelloServer : public Application
{
public:
	/**
	 * 析构函数
	 **/
	virtual ~HelloServer() {};

	/**
	 * 服务初始化
	 **/
	virtual void initialize();

	/**
	 * 服务销毁
	 **/
	virtual void destroyApp();

public:
    /**
     * CCServer的对象名称
     */
    static string CCSERVER_OBJ;
};
////////////////////////////////////////////
#endif
