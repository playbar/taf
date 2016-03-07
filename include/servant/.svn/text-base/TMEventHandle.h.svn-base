#ifndef __TAF_TM_EVENT_HANDLE_H_
#define __TAF_TM_EVENT_HANDLE_H_

#include "util/tc_autoptr.h"
#include <set>

namespace taf
{
/**
 * 超时事件处理器的基类
 */
class TMEventHandle : virtual public TC_HandleBase
{
public:
    /**
     * 虚析构函数
     */
	virtual ~TMEventHandle() {}

    /**
     * 超时处理方法，缺省不处理
     * @return int
     */
	virtual int handleTimeout() { return 0; }
};
///////////////////////////////////////////////////////
}
#endif
