#include "hessian/HessianOutput.h"
#include <cassert>

namespace taf
{

string HessianOutput::start(const string& method_name)
{
	string mc("c");

	unsigned short mlen  = method_name.length();
	unsigned char mlen16 = mlen >> 8;
	unsigned char mlen8  = mlen & 0x00FF;

	mc.append(1, (char)1);
	mc.append(1, (char)0);
	mc.append(1, 'm');
	mc.append(1, mlen16);
	mc.append(1, mlen8);
	mc.append(method_name);
	return mc;
}

string& HessianOutput::complete(string& call)
{
	call.append(1, 'z');
	return call;
}

string& HessianOutput::setParameter(string& call, const HObjectPtr &object)
{
	return writeObject(call, object);
}

string& HessianOutput::writeObject(string& call, const HObjectPtr &object)
{
    assert(object);
    return object->write(call);
}

}

