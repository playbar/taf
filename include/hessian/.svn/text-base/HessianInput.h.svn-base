#ifndef _HESSIAN_INPUT_H
#define _HESSIAN_INPUT_H

#include "hessian/HessianWrappers.h"
#include "hessian/HessianStringStream.h"
#include <string>
#include <list>
#include <vector>
#include <memory>

using namespace std;

namespace taf
{

/**
 * 解析响应包
 */
class HessianInput
{
public:
    /**
     * 构造函数
     * @param buffer
     */
	HessianInput(const string &buffer)
    {
		_hssPtr = new HessianStringStream(buffer);
	}

    /**
     * 获取结果
     *
     * @return HObjectPtr
     */
	HObjectPtr result();

protected:

    /**
     * 返回异常
     * @param expect
     * @param ch
     *
     * @return HessianException
     */
	HessianException expect(const string& expect, int ch);

protected:
    /**
     * 数据流
     */
	HessianStringStreamPtr  _hssPtr;
};

}

#endif

