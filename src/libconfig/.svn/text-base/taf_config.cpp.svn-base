#include "config/taf_config.h"
#include "util/tc_file.h"
#include "servant/Communicator.h"
#include "notify/taf_notify.h"
#include <fstream>

namespace taf
{

int TafRemoteConfig::setConfigInfo(const CommunicatorPtr &comm, const string &obj, const string & app, const string &serverName, const string& basePath, int maxBakNum)
{
    _comm           = comm;
    if(!obj.empty())
    {
        _configPrx      = _comm->stringToProxy<ConfigPrx>(obj);
    }
    _sApp           = app;
    _sServerName    = serverName;
    _sBasePath      = basePath;
    _iMaxBakNum     = maxBakNum;

    return 0;
}

bool TafRemoteConfig::addConfig(const string & sFileName, string &buffer, bool bAppConfigOnly)
{
	TC_LockT<TC_ThreadMutex> lock(_mutex);

    try
    {
        string sFullFileName = _sBasePath + "/" + sFileName;

        string newFile = getRemoteFile(sFileName, bAppConfigOnly);

        if (newFile.empty() || access(newFile.c_str(), R_OK) != 0)//拉取不到配置中心的配置文件
        {
            if(access(sFullFileName.c_str(), R_OK) == 0) //获取本地配置成功，返回成功，但需要告警一下。
            {
                buffer = "[fail] get remote config:" + sFileName + "fail,use the local config.";

                return true;
            }
            throw runtime_error("access file error:" + newFile);
        }

        if (TC_File::load2str(newFile) != TC_File::load2str(sFullFileName))
        {
            for (int i = _iMaxBakNum - 1; i >= 1; --i)
            {
                if (access(index2file(sFullFileName, i).c_str(), F_OK) == 0)
                {
                    localRename(index2file(sFullFileName, i), index2file(sFullFileName, i+1));
                }
            }

            if (access(sFullFileName.c_str(), F_OK) == 0)
            {
                localRename(sFullFileName, index2file(sFullFileName, 1));
            }
        }

        localRename(newFile, sFullFileName);

        assert(!access(sFullFileName.c_str(), R_OK));

        buffer = "[succ] get remote config:" + sFileName;

        return true;
    }
    catch (std::exception& e)
    {
        buffer = "[fail] get remote config '" + sFileName + "' error:" + string(e.what());
    }
    catch (...)
    {
        buffer = "[fail] get remote config '" + sFileName + "' unknown error";
    }

    return false;
}

string TafRemoteConfig::getRemoteFile(const string &sFileName, bool bAppConfigOnly)
{
    if (_configPrx)
    {
        string stream;
        int ret = _configPrx->loadConfig(_sApp, (bAppConfigOnly ? "" : _sServerName), sFileName, stream);
        if (ret != 0 || stream.empty())
        {
            throw runtime_error("remote config file is empty:" + sFileName);
        }

        string newFile = _sBasePath + "/" + sFileName + "." + TC_Common::tostr(time(NULL));

        std::ofstream out(newFile.c_str());
        
        string result;
        if (out)
        {
            out << stream;//如果硬盘满了，是否能写入成功需要进行判断。
            if(out.bad()) 
            {
                result = "[fail] copy stream to disk error." ;
                TafRemoteNotify::getInstance()->report(result);
                return "";
            }
            else
            {
                return newFile;
            }
        }
    }
    return "";
}

string TafRemoteConfig::index2file(const string & sFullFileName, int index)
{
    return   sFullFileName + "." + TC_Common::tostr(index) + ".bak";
}

void TafRemoteConfig::localRename(const string& oldFile, const string& newFile)
{
    if (::rename(oldFile.c_str(), newFile.c_str()) != 0)
    {
        throw runtime_error("rename file error:" + oldFile + "->" + newFile);
    }
}

string TafRemoteConfig::recoverSysConfig(const string & sFullFileName)
{
    try
    {
        for (int i = 1; i <= _iMaxBakNum; ++i)
        {
            if (access(index2file(sFullFileName,i).c_str(), R_OK) == 0)
            {
                localRename(index2file(sFullFileName, i), sFullFileName);

                return "[succ] recover file:" + index2file(sFullFileName, i);
            }
        }
    }
    catch (std::exception& e)
    {
        return "[fail] recover config error:" + string(e.what());
    }
    catch (...)
    {
        return "[fail] recover config error";
    }

    return "[fail] no backup file.";
}

}


