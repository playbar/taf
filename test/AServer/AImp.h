#ifndef _AImp_H_
#define _AImp_H_

#include "servant/Application.h"
#include "A.h"
#include "log/taf_logger.h"

using namespace taf;

/**
 *
 *
 */
class AImp : public Test::A
{
public:
    AImp()
	{
			/*
			for(int i = 0; i< 20000; i++)
			{
					_abc += "a";
			}
			*/
	}
	/**
	 *
	 */
	virtual ~AImp() {}

	/**
	 *
	 */
	virtual void initialize();

	/**
	 *
	 */
    virtual void destroy();

	/**
	 *
	 */

	virtual std::string test(taf::Int64 p,taf::Int32 i,taf::JceCurrentPtr current)
	{
	//	sleep(rand() % 5);
		LOG->debug() << p << ":" << i << endl;

//		LOG->debug() << current->getRequestId() << endl;
		return TC_Common::tostr(i);
	}

    bool load(const string& command, const string& params, string& result);

protected:

    string _abc;

};
/////////////////////////////////////////////////////
#endif
