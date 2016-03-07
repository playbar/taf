#ifndef _DemoServer_H_
#define _DemoServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace taf;

/**
 *
 **/
class DemoServer : public Application
{
public:
	/**
	 *
	 **/
	virtual ~DemoServer() {};

	/**
	 *
	 **/
	virtual void initialize();

	/**
	 *
	 **/
	virtual void destroyApp();
};

extern DemoServer g_app;

////////////////////////////////////////////
#endif
