#ifndef _HESSIAN_OUTPUT_H
#define _HESSIAN_OUTPUT_H

#include "hessian/HessianWrappers.h"

#include <string>

namespace taf
{

/**
 * 编码发送的数据
 */
class HessianOutput
{
public:
    /**
     * 开始调用
     * @param method_name
     *
     * @return string
     */
	string start(const string& method_name);

    /**
     * 完成调用
     * @param call
     *
     * @return string&
     */
	string& complete(string& call);

    /**
     * 设置参数
     * @param call
     * @param object
     *
     * @return string&
     */
	string& setParameter(string& call, const HObjectPtr &object);

protected:

    /**
     * 写object类型
     * @param call
     * @param object
     *
     * @return string&
     */
	string& writeObject(string& call, const HObjectPtr &object);
};

}

#endif

