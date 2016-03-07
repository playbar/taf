#include "hessian/HessianInput.h"

namespace taf
{

HObjectPtr HessianInput::result()
{
    //先读取头部信息
	int tag = _hssPtr->read();
    //html错误
    if(tag == '<')
    {
        throw HessianProtocolException("HessianProtocolException::" + _hssPtr->buffer());
    }

	if (tag != 'r')
    {
		throw expect("taf reply", tag);
    }

	int major = _hssPtr->read();
	int minor = _hssPtr->read();
	if (major & minor);	// avoid warning

    //读取返回值
    HObjectPtr oPtr;
    tag = _hssPtr->read();

    oPtr = HObject::buildHObject(_hssPtr, tag);

    //读取结束信息
    tag = _hssPtr->read();
    if (tag != 'z')
    {
        throw expect("end of reply", tag);
    }

    return oPtr;
}

HessianException HessianInput::expect(const string& expect, int ch)
{
	if (ch < 0)
		return HessianProtocolException("HessianProtocolException::expected '" + expect + "' at end of file");
	else
		return HessianProtocolException("HessianProtocolException::expected '" + expect + "' at " + (char) ch);
}

}


