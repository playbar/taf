#ifndef __TC_TIME_PROVIDER_H_
#define __TC_TIME_PROVIDER_H_

#include <string>
#include <string.h>
#include "util/tc_monitor.h"
#include "util/tc_thread.h"
#include "util/tc_autoptr.h"

#define rdtsc(low,high) \
     __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

namespace taf
{
/////////////////////////////////////////////////
// 说明: 秒级、微妙级时间提供类
// Author : j@syswin.com    
// Modify : skingfan@tencent.com                        
/////////////////////////////////////////////////
class TC_TimeProvider;

typedef TC_AutoPtr<TC_TimeProvider> TC_TimeProviderPtr;

/**
 * 提供秒级别的时间
 */
class TC_TimeProvider : public TC_Thread, public TC_HandleBase
{
public:

    /**
     * 获取实例
     * @return TimeProvider&
     */
    static TC_TimeProvider* getInstance();

    /**
     * 构造函数
     */
    TC_TimeProvider() : _terminate(false),_use_tsc(true),_cpu_cycle(0),_buf_idx(0)
    {
        memset(_t,0,sizeof(_t));
        memset(_tsc,0,sizeof(_tsc));

        struct timeval tv;
        ::gettimeofday(&tv, NULL);
        _t[0] = tv;
        _t[1] = tv;
    }

    /**
     * 析构，停止线程
     */
    ~TC_TimeProvider(); 

    /**
     * 获取时间
     *
     * @return time_t
     */
    time_t getNow()     {  return _t[_buf_idx].tv_sec; }

    /**
     * 获取时间
     *
     * @para timeval 
     *  
     * @return void 
     */
    void getNow(timeval * tv);
    
    /**
     * 获取cpu主频
     *  
     * @return float 
     */  

    float cpuMHz();

    /**
     * 运行
     */
protected:

    virtual void run();

    static TC_ThreadLock        g_tl;

    static TC_TimeProviderPtr   g_tp;

private:
    void setTsc(timeval& tt);

    void addTimeOffset(timeval& tt,const int &idx);

protected:

    bool    _terminate;

    bool    _use_tsc;

private:
    float           _cpu_cycle; 

    volatile int    _buf_idx;

    timeval         _t[2];

    uint64_t        _tsc[2];  
};

}

#endif


