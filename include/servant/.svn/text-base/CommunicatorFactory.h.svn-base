#ifndef __TAF_COMMUNICATOR_FACTORY_H_
#define __TAF_COMMUNICATOR_FACTORY_H_

#include "servant/Global.h"
#include "servant/Communicator.h"

namespace taf
{
/**
 * 创建CommunicatorPtr对象
 */
    
class CommunicatorFactory : public TC_Singleton<CommunicatorFactory>, public TC_HandleBase, public TC_ThreadRecMutex
{
public:
    /**
     * 构造函数
     * @param comm
     */
    CommunicatorFactory(){};

    /**
     * 析构
     */
    ~CommunicatorFactory(){};

    /**
     * 获取CommunicatorPtr对象
     * @param name
     * @return ServantPrx
     */
    CommunicatorPtr getCommunicator(const string& name = "default")
    {
        TC_LockT<TC_ThreadRecMutex> lock(*this);

        map<string, CommunicatorPtr>::iterator it = _mComms.find(name);

        if (it == _mComms.end())
        {
            _mComms[name] = new Communicator();

            it = _mComms.find(name);
        }
        return it->second;
    }
    
     /**
     * 获取CommunicatorPtr对象 
     * @param conf 
     * @param name
     * @return ServantPrx
     */
    CommunicatorPtr getCommunicator(TC_Config& conf, const string& name = "default")
    {
        TC_LockT<TC_ThreadRecMutex> lock(*this);

        map<string, CommunicatorPtr>::iterator it = _mComms.find(name);

        if (it == _mComms.end())
        {
            _mComms[name] = new Communicator(conf);

            it = _mComms.find(name);

            return it->second;
        }

        string s = "";

        it->second->setProperty(conf);

        it->second->reloadProperty(s);

        return it->second;
    }

private:
  
    /**
     * 已创建的对象
     */
    map<string, CommunicatorPtr> _mComms;
};
//////////////////////////////////////////////////////
}

#endif
