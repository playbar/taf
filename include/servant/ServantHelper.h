#ifndef __TAF_SERVANT_HELPER_H
#define __TAF_SERVANT_HELPER_H

#include <iostream>
#include <map>
#include <vector>
#include "util/tc_autoptr.h"
#include "util/tc_singleton.h"
#include "servant/Servant.h"

namespace taf
{

/**
 * Servant
 */
class ServantHelperCreation : public TC_HandleBase
{
public:
    virtual ServantPtr create(const string &s) = 0;
};
typedef TC_AutoPtr<ServantHelperCreation> ServantHelperCreationPtr;

/**
 * Servant
 */
template<class T>
struct ServantCreation : public ServantHelperCreation
{
    ServantPtr create(const string &s) { T *p = new T; p->setName(s); return p; }
};

//////////////////////////////////////////////////////////////////////////////
//
/**
 * Servant管理
 */
class ServantHelperManager : public TC_Singleton<ServantHelperManager>
{
public:

    ServantHelperManager() : _isDyeing(false)
    {
    }

    /**
     * 添加Servant
     * @param T
     * @param id
     */
    template<typename T>
    void addServant(const string &id)
    {
        _servant_creator[id] = new ServantCreation<T>();
    }

    /**
     * 生成Servant
     * @param id
     *
     * @return ServantPtr
     */
    ServantPtr create(const string &sAdapter);

    /**
     * 添加Adapter的Servant
     * @param sAdapter
     * @param sServant
     */
    void setAdapterServant(const string &sAdapter, const string &sServant);

    /**
     * 根据adapter名称获取Servant名称
     * @param sAdapter
     * @return string
     */
    string getAdapterServant(const string &sAdapter)
    {
        if(_adapter_servant.find(sAdapter) != _adapter_servant.end())
        {
            return _adapter_servant[sAdapter];
        }
        return "(NO TAF PROTOCOL)";
    }

    /**
     * 根据servant名称获取adapter名称
     * @param sServant 
     * @return string 
     */
    string getServantAdapter(const string& sServant)
    {
        if(_servant_adapter.find(sServant) != _servant_adapter.end())
        {
            return _servant_adapter[sServant];
        }
        return "";
    }

    /**
     * 获取Adapter/Servant对应表
     * @return map<string, string>
     */
    map<string, string> getAdapterServant() {return _adapter_servant;}

    /**
     * 设置染色信息
     * @param params: notify的输入参数
     * @param sDyeingKey:      用户号码
     * @param sDyeingServant:  对象名称
     * @param sDyeingInterface:接口名称
     * @return string: 设置结果
     */
    bool setDyeing(const string & sDyeingKey, const string & sDyeingServant, const string & sDyeingInterface);

    /**
     * 是否是染色的请求
     * @param sKey:      用户号码
     * @param sServant:  对象名称
     * @param sInterface:接口名称
     * @return string: 设置结果
     */
    bool isDyeingReq(const string & sKey, const string & sServant, const string & sInterface);

    /** 
     * 是否是已经被染色
     */
    bool isDyeing() {return _isDyeing;}

    /**
     * 设置路由器 
     * @param sServant 
     * @param router 
     * @param router
     */
    bool setRouter(const string & sServant,ServantProxyRouterPtr router);

    /**
     * 根据key获取用户的灰度状态
     * 返回INVALID_GRID_CODE则认为无灰度状态 
     * @param sServant 
     * @param key
     * @return int
     */
    int getGridByKey(const string & sServant,const string & sKey);


protected:

    /**
     * Servant生成类
     */
    map<string, ServantHelperCreationPtr>   _servant_creator;

    /**
     * Adapter包含的Servant(Adapter名称:servant名称)
     */
    map<string, string>                     _adapter_servant;

    /**
     * Adapter包含的Servant(Servant名称:Adapter名称)
     */
    map<string, string>                     _servant_adapter;

protected:

    /**
     * 锁
     */
    TC_ThreadMutex _mutex;

    /**
     * 是否染色
     */
    bool   _isDyeing;

    /**
     * 染色用户号码
     */
    string  _sDyeingKey;
 
    /**
     * 染色的servant对象
     */
    string  _sDyeingServant;

    /**
     * 染色的接口
     */
    string _sDyeingInterface;

protected:

    /**
     * 服务端灰度路由
     */
    map<string,ServantProxyRouterPtr> _mRouter;


};

}

#endif

