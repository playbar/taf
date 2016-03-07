#include "AImp.h"

using namespace std;

//////////////////////////////////////////////////////
void AImp::initialize()
{
	//initialize servant here:
	//...
	LOG->debug() << "initialize load:" << this << endl;
	TAF_ADD_ADMIN_CMD_NORMAL("load", AImp::load);
}

//////////////////////////////////////////////////////
void AImp::destroy()
{
	//destroy servant here:
	//...
}

bool AImp::load(const string& command, const string& params, string& result)
{
	LOG->debug() << "load:" << this << endl;
	_abc = "abc";
	result = _abc;

	return true;
}
