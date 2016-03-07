#ifndef __APP_CACHE_H_
#define __APP_CACHE_H_

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "util/tc_singleton.h"
#include "util/tc_config.h"
#include "util/tc_timeprovider.h"
#include "util/tc_file.h"
#include "util/tc_thread_mutex.h"
#include "log/taf_logger.h"

#define APPCACHE_ROOT_PATH "/cache"

using namespace std;

namespace taf
{

class AppCache;

//////////////////////////////////////////////////////////////////////
// 缓存
class AppCache : public TC_Singleton<AppCache>, public TC_ThreadMutex
{    
public:
    AppCache()
    : _tLastSynTime(0)
    ,_iSynInterval(1000)
    {
    }
    ~AppCache()
    {
    }
     /**
     * 设置本地信息
     * @param sFilePath, cache文件路径
     * @param iSynInterval(ms), 同步间隔时间
     */
    void setCacheInfo(const string &sFile,int32_t iSynInterval = 1000)
    {   
        try
        {
            TC_LockT<TC_ThreadMutex> lock(*this);            

            string sPath = TC_File::extractFilePath(sFile);    

            TC_File::makeDirRecursive(sPath);        

            _sFile = sFile;

            _iSynInterval = iSynInterval;

            if (TC_File::isFileExistEx(_sFile))
            {
                _tFileCache.parseFile(_sFile);                 
            } 

            //如果是旧版本数据（无版本号)直接清理
            if (_tFileCache.get(string(APPCACHE_ROOT_PATH)+"<tafversion>","") == "")
            {
                TC_Config tFileCache;  

                _tFileCache = tFileCache;
            }
        } 
        catch(exception &e)
        {
            LOG->error() <<"[TAF][AppCache setCacheInfo "<<e.what()<<"]"<<endl;
        }

    }

    /**
     * 从cache文件获取服务列表
     * @param sObjName 
     * @param sDomain 
     * @return string
     */
    string get(const string & sName,const string sDomain = ""/*=APPCACHE_ROOT_PATH*/)
    {    
        if(_sFile.empty())
        {
            return "";
        }    
        try
        {
           TC_LockT<TC_ThreadMutex> lock(*this);
            string sValue = _tFileCache.get(string(APPCACHE_ROOT_PATH)+"/"+sDomain+"<"+sName+">"); 
            return sValue;
        } 
        catch(exception &e)
        {
            LOG->error() <<"[TAF][AppCache get"<<sName<<","<<e.what()<<"]"<<endl;
        }
        return "";
    }
    
     /**
     * 设置数据到cache文件
     * @param sName 
     * @param sValue
     * @param sDomain
     * @return int
     */
    int set(const string &sName,const string &sValue,const string sDomain = ""/*=APPCACHE_ROOT_PATH*/)
    {   
        if(_sFile.empty())
        {
            return -1;
        }
        try
        {
            TC_LockT<TC_ThreadMutex> lock(*this);                   
            map<string, string> m;  
            m[sName] = sValue;
            TC_Config tConf;
            tConf.insertDomainParam(string(APPCACHE_ROOT_PATH)+"/"+sDomain,m,true);   
            if(_tLastSynTime == 0)   //第一次写数据 打印tafversion
            {   
                m.clear(); 
                m["tafversion"] = string(TAF_VERSION);
                tConf.insertDomainParam(string(APPCACHE_ROOT_PATH),m,true); 
            }           
            _tFileCache.joinConfig(tConf,true);
                 
            time_t now = TC_TimeProvider::getInstance()->getNow();
            if(_tLastSynTime + _iSynInterval/1000 > now)
            {
                return 0;
            }
            _tLastSynTime = now;      
            TC_File::save2file(_sFile,_tFileCache.tostr());
            return 0;
        } 
        catch(exception &e)
        {
            LOG->error() <<"[TAF][AppCache set name "<<sName<<",value"<<sValue<<","<<e.what()<<"]"<<endl;
        }
        return -1;
    }
    
private:
        
    string      _sFile;
    TC_Config   _tFileCache;      
    time_t      _tLastSynTime;   
    int32_t     _iSynInterval;
};
//////////////////////////////////////////////////////////////////////
}
#endif
