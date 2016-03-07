#ifndef __TAF_STAT_REPORT_H_
#define __TAF_STAT_REPORT_H_

#include "util/tc_thread.h"
#include "servant/PropertyReport.h"
#include "servant/StatF.h"
#include "servant/PropertyF.h"

/*
    属性上报使用方式:
    StatReport report;
    report->setReportInfo( pPtr,strMasterName,strMasterIp, 5 );
    vector<int> v;
    v.push_back(1000);
    v.push_back(1001);
    v.push_back(1002);
    v.push_back(1003);
    v.push_back(1004);

    PropertyReportPtr srPtr = report->createPropertyReport("test", PropertyReport::sum(), PropertyReport::dist(v));
    srPtr->report(i);
*/

namespace taf
{

/**
 * 状态上报类, 上报的信息包括:
 * 1 模块间调用的信息
 * 2 业务自定义的属性统计
 */
struct StatSampleMsgHead
{
    string slaveName;
    string interfaceName;
    string ip;
    bool operator <(const StatSampleMsgHead& m)const
    {
        if(slaveName != m.slaveName)
        {
            return slaveName < m.slaveName;
        }
        if(interfaceName != m.interfaceName)
        {
            return interfaceName < m.interfaceName;
        }
        return ip < m.ip;
    }
};
class StatReport : public TC_HandleBase, public TC_Thread, public TC_ThreadLock
{
public:
    typedef  map<StatMicMsgHead, StatMicMsgBody>        MapStatMicMsg;
    typedef  map<StatPropMsgHead, StatPropMsgBody>      MapStatPropMsg;
    typedef  multimap<StatSampleMsgHead,StatSampleMsg>  MMapStatSampleMsg;

    enum StatResult
    {
        STAT_SUCC       = 0,
        STAT_TIMEOUT    = 1,
        STAT_EXCE       = 2,
    };
public:
    /**
     * 构造函数
     */
    StatReport();

    /**
     * 析够函数
     */
    ~StatReport();

    /**
     * 初始化
     * @param statPrx, 模块间调用服务器地址
     * @param propertyPrx, 用户自定义属性服务器地址
     * @param strModuleName, 模块名
     * @param strModuleIp, 模块ip
     * @param iReportInterval, 上报间隔单位秒
     * @param iMaxReporSize一次最大上报包长度。 跟udp最大允许包8k、MTU长度1472有关，暂定取值范围[500-1400] 
     * @param iSampleRate, 采样比率1/1000 
     * @param iMaxSampleCount, 最大采样数 
     */
    void setReportInfo(const StatFPrx& statPrx,
                       const PropertyFPrx& propertyPrx,
                       const string& strModuleName,
                       const string& strModuleIp,
                       int iReportInterval = 60,
                       int iSampleRate = 1000,
                       unsigned int iMaxSampleCount = 100,
                       int iMaxReportSize = 1400);

    /**
     * 设置模块间调用数据
     * @param strModuleName, 被调模块. 一般采用app.servername 例如:Comm.BindServer
     * @param strInterfaceName,被调接口.一般为函数名
     * @param strModuleIp,被调ip
     * @param shSlavePort,被调port
     * @param eResult,成功STAT_SUCC，超时 STAT_TIMEOUT，异常STAT_EXC.
     * @param iSptime,耗时
     * @param iReturnValue,返回值
     * @param bFromClient,从客户端采集 false从服务端采集
     * 。
     */
    void report(const string& strModuleName,
                const string& strInterfaceName,
                const string& strModuleIp,
                uint16_t iPort,
                StatResult eResult,
                int  iSptime,
                int  iReturnValue = 0,
                bool bFromClient = true);

     /**
     * 设置模块间调用数据 
     * @param strMasterName     主调用方的名字
     * @param strMasterIp       主调用方的地址
     * @param strSlaveName      被调用方的名字
     * @param strSlaveIp        被调用方的地址 
     * @param iSlavePort        被调用方的端口 
     * @param strInterfaceName  被调接口.一般为函数名
     * @param eResult           成功STAT_SUCC，超时 STAT_TIMEOUT，异常STAT_EXC.
     * @param iSptime           耗时(单位毫秒)
     * @param iReturnValue      返回值
     */
    void report(const string& strMasterName,
                const string& strMasterIp,
                const string& strSlaveName,
                const string& strSlaveIp,
                uint16_t iSlavePort,
                const string& strInterfaceName,
                StatResult eResult,
                int  iSptime,
                int  iReturnValue = 0);

    /**
     * 根据名字获取属性上报对象
     * @param sProperty
     *
     * @return PropertyReportPtr
     */
    PropertyReportPtr getPropertyReport(const string& strProperty)
    {
         Lock lock(*this);
         if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
         {
             return _mStatPropMsg[strProperty];
         }

         return NULL;
    }

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     *
     * @return PropertyReportPtr
     */
    template<typename T1>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
              
         PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1>::Result>(t1);
                           
         _mStatPropMsg[strProperty] = srPtr;

         return srPtr;
    }

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2>::Result>(t1, t2);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3>::Result>(t1, t2, t3);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     * @param t4
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3, typename T4>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3, T4 t4)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3, T4>::Result>(t1, t2, t3, t4);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     * @param t4
     * @param t5
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3, T4, T5>::Result>(t1, t2, t3, t4, t5);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     * @param t4
     * @param t5
     * @param t6
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3, T4, T5, T6>::Result>(t1, t2, t3, t4, t5, t6);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     * @param t4
     * @param t5
     * @param t6
     * @param t7
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3, T4, T5, T6, T7>::Result>(t1, t2, t3, t4, t5, t6, t7);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     * @param t4
     * @param t5
     * @param t6
     * @param t7
     * @param t8
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
    {
        Lock lock(*this);
        
        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }
        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3, T4, T5, T6, T7, T8>::Result>(t1, t2, t3, t4, t5, t6, t7, t8);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     * @param t4
     * @param t5
     * @param t6
     * @param t7
     * @param t8
     * @param t9
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
    {
        Lock lock(*this);

        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }

        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3, T4, T5, T6, T7, T8, T9>::Result>(t1, t2, t3, t4, t5, t6, t7, t8, t9);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

    /**
     * 生成属性上报对象
     * @param strProperty
     * @param t1
     * @param t2
     * @param t3
     * @param t4
     * @param t5
     * @param t6
     * @param t7
     * @param t8
     * @param t9
     * @param t10
     *
     * @return PropertyReportPtr
     */
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    PropertyReportPtr createPropertyReport(const string& strProperty, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10)
    {
        Lock lock(*this);

        if(_mStatPropMsg.find(strProperty) != _mStatPropMsg.end())
        {
            return _mStatPropMsg[strProperty];
        }

        PropertyReportPtr srPtr = new PropertyReportImp<typename TL::TLMaker<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::Result>(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);

        _mStatPropMsg[strProperty] = srPtr;

        return srPtr;
    };

public:

    /* 
    * 获取stat代理
    */
    StatFPrx getStatPrx() {return _statPrx; }
    
    /*
    * 采样
    */
    void doSample(const string& strSlaveName,
                      const string& strInterfaceName,
                      const string& strSlaveIp,
                      map<string, string> &status);
    /*
    * 采样id
    */
    string sampleUnid();

    /**
     * 增加关注时间点.  调用方式addStatInterv(5)
     * @param strTimePoint  时间点序列
     * @return
     */
    void addStatInterv(int  iInterv);

    /**
     * 重置关注时间点.
     * @return
     */
    void resetStatInterv();

    /**
     * 结束线程
     */
    void terminate();

    /**
     * 轮询函数
     */
    virtual void run();
    
private:

    /**
     * 限制长度
     * @param str
     * @param limitlen
     *
     * @return string
     */
    string trimAndLimitStr(const string& str, uint32_t limitlen);

    /**
     *
     * @param head
     * @param body
     * @param msg
     * @param bFromClient
     */
    void submit(StatMicMsgHead& head, StatMicMsgBody& body, bool bFromClient);

    /**
     *
     * @param body
     * @param time
     * @return void
     */
    void getIntervCount(int time,StatMicMsgBody& body);

    /**
     * 上报模块间调用信息  Mic = module interval call
     * @param msg
     * @param bFromClient  是否为客户端上报
     * @return int
     */
    int reportMicMsg(MapStatMicMsg &msg, bool bFromClient);

    /**
     * 上报属性信息  Prop = property
     * @return int
     */
    int reportPropMsg();
    
    /**
     * stat 采样
     */
    int reportSampleMsg();
    
private:
    time_t              _tTime;

    int                 _iReportInterval;

    int                 _iMaxReportSize;

    bool                _bTerminate;

    string              _strModuleName;

    string              _strIp;
    
    int                 _iSampleRate; //生成模块间调用时序图的采样比率
    
    unsigned int        _iMaxSampleCount; //1分钟内最大采样条数

    StatFPrx            _statPrx;

    MapStatMicMsg       _mStatMicMsgClient;

    MapStatMicMsg       _mStatMicMsgServer;
    
    MMapStatSampleMsg   _mmStatSampleMsg;

    vector<int>         _vTimePoint;

    PropertyFPrx        _propertyPrx;
    
    map<string, PropertyReportPtr>          _mStatPropMsg;

private:
    //map<string,*objectProxy>  _mObjectProxy; //回设stat数据到objectProxy供业务使用。 
    
};
///////////////////////////////////////////////////////////
}

#endif
