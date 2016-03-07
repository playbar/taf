#ifndef __TAF_BASE_NOTIFY_H_
#define __TAF_BASE_NOTIFY_H_

#include "servant/Global.h"
#include "util/tc_thread_queue.h"
#include "util/tc_thread_mutex.h"
#include "util/tc_functor.h"
#include "util/tc_loki.h"

//////////////////////////////////////////////////////////////////////
namespace taf
{

/**
 * 需要接收到管理命令的对象从该类派生
 */
class BaseNotify : public TC_ThreadRecMutex, public TC_HandleBase
{
public:
    /**
     * 构造函数
     */
    BaseNotify();

    /**
     * 析构函数
     */
    virtual ~BaseNotify();

    /**
     * 接收管理命令
     * @param command
     * @param params
     * @param current
     * @param result
     * @return bool
     */
    bool notify(const string& command, const string& params, JceCurrentPtr current, string& result);

    /**
     * 处理命令的函数类型
     * bool: true(继续往后通知其他object),false(通知中止)
     * result: 处理结果描述
     */
    typedef TC_Functor<bool, TL::TLMaker<const string&, const string&, string&>::Result> TAdminFunc;

    /**
     * 添加Servant管理命令和对应的处理方法
     * @param command
     * @param func
     */
    void addAdminCommandNormal(const string& command, TAdminFunc func);

    /**
     * 添加管理命令和对应的处理方法
     * 提前于Servant对象执行收到的命令
     * @param command
     * @param func
     */
    void addAdminCommandPrefix(const string& command, TAdminFunc func);

protected:
    /**
     * 命令处理方法
     */
    map<string, TAdminFunc> _procFunctors;
};
/////////////////////////////////////////////////////////////////////
}
#endif
