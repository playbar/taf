#ifndef __TAF_APPLICATION_H
#define __TAF_APPLICATION_H

#include <iostream>
#include <set>
#include <signal.h>

#include "util/tc_autoptr.h"
#include "util/tc_config.h"
#include "util/tc_epoll_server.h"
#include "servant/BaseNotify.h"
#include "servant/ServantHelper.h"
#include "servant/ServantHandle.h"
#include "servant/StatReport.h"
#include "servant/CommunicatorFactory.h"
#include "log/taf_logger.h"
#include "config/taf_config.h"
#include "notify/taf_notify.h"

namespace taf
{

/**
 * 以下定义配置框架支持的命令
 */
#define TAF_CMD_LOAD_CONFIG         "taf.loadconfig"        //从配置中心, 拉取配置下来: taf.loadconfig filename
#define TAF_CMD_SET_LOG_LEVEL       "taf.setloglevel"       //设置滚动日志的等级: taf.setloglevel [NONE, ERROR, WARN, DEBUG]
#define TAF_CMD_VIEW_STATUS         "taf.viewstatus"        //查看服务状态
#define TAF_CMD_VIEW_VERSION        "taf.viewversion"       //查看服务采用TAF的版本
#define TAF_CMD_CONNECTIONS         "taf.connection"        //查看当前链接情况
#define TAF_CMD_LOAD_PROPERTY       "taf.loadproperty"      //使配置文件的property信息生效
#define TAF_CMD_VIEW_ADMIN_COMMANDS "taf.help"              //帮助查看服务支持的管理命令
#define TAF_CMD_SET_DYEING          "taf.setdyeing"         //设置染色信息: taf.setdyeing key servant [interface]
#define TAF_CMD_CLOSE_COUT          "taf.closecout"         //设置是否启关闭cout\cin\cerr: taf.openthreadcontext yes/NO 服务重启才生效

/**
 * 通知信息给notify服务, 展示在页面上
 */
//上报普通信息
#define TAF_NOTIFY_NORMAL(info)     {TafRemoteNotify::getInstance()->notify(NOTIFYNORMAL, info);}

//上报警告信息
#define TAF_NOTIFY_WARN(info)       {TafRemoteNotify::getInstance()->notify(NOTIFYWARN, info);}

//上报错误信息
#define TAF_NOTIFY_ERROR(info)      {TafRemoteNotify::getInstance()->notify(NOTIFYERROR, info);}

//发送心跳给node 多个adapter分别上报
#define TAF_KEEPALIVE(adapter)      {TafNodeFHelper::getInstance()->keepAlive(adapter);}

//发送TAF版本给node
#define TAF_REPORTVERSION(x)        {TafNodeFHelper::getInstance()->reportVersion(TAF_VERSION);}

/**
 * 添加前置的命令处理方法
 * 在所有Normal方法之前执行
 * 多个前置方法之间顺序不确定
 */
#define TAF_ADD_ADMIN_CMD_PREFIX(c,f) \
    do { addAdminCommandPrefix(string(c), TAdminFunc(this, &f)); } while (0);

/**
 * 添加Normal命令处理方法
 * 在所有前置方法最后执行
 * 多个Normal方法之间顺序不确定
 */
#define TAF_ADD_ADMIN_CMD_NORMAL(c,f) \
    do { addAdminCommandNormal(string(c), TAdminFunc(this, &f)); } while (0);

/**
 * 服务基本信息
 */
struct ServerConfig
{
    static std::string Application;         //应用名称
    static std::string ServerName;          //服务名称,一个服务名称含一个或多个服务标识
    static std::string BasePath;            //应用程序路径，用于保存远程系统配置的本地目录
    static std::string DataPath;            //应用程序数据路径用于保存普通数据文件
    static std::string LocalIp;             //本机IP
    static std::string LogPath;             //log路径
    static int         LogSize;             //log大小(字节)
    static int         LogNum;              //log个数()
    static std::string Local;               //本地套接字
    static std::string Node;                //本机node地址
    static std::string Log;                 //日志中心地址
    static std::string Config;              //配置中心地址
    static std::string Notify;              //信息通知中心
    static std::string ConfigFile;          //框架配置文件路径
    static int         ReportFlow;          //是否服务端上报所有接口stat流量 0不上报 1上报(用于非taf协议服务流量统计)
};

/**
 * 服务的基类
 */
class Application : public BaseNotify
{
public:
    /**
     * 应用构造
     */
    Application();

    /**
     * 应用析构
     */
    virtual ~Application();

    /**
     * 初始化
     * @param argv
     */
    void main(int argc, char *argv[]);

    /**
     * 运行
     */
    void waitForShutdown();

public:
    /**
     * 获取配置文件
     *
     * @return TC_Config&
     */
    static TC_Config& getConfig();

    /**
     * 获取通信器
     *
     * @return CommunicatorPtr&
     */
    static CommunicatorPtr& getCommunicator();

    /**
     * 获取服务Server对象
     *
     * @return TC_EpollServerPtr&
     */
    static TC_EpollServerPtr& getEpollServer();

    /**
     *  中止应用
     */
    static void terminate();

	/**
     * 添加Config
     * @param filename
     */
    bool addConfig(const string &filename);

    /**
     * 添加应用级的Config
     * @param filename
     */
    bool addAppConfig(const string &filename);

protected:
    /**
     * 初始化, 只会进程调用一次
     */
    virtual void initialize() = 0;

    /**
     * 析够, 进程只会调用一次
     */
    virtual void destroyApp() = 0;

    /**
     * 处理加载配置的命令
     * 处理完成后继续通知Servant
     * @param filename
     */
    bool cmdLoadConfig(const string& command, const string& params, string& result);

    /**
     * 设置滚动日志等级
     * @param command
     * @param params
     * @param result
     *
     * @return bool
     */
    bool cmdSetLogLevel(const string& command, const string& params, string& result);

    /**
     * 查看服务状态
     * @param command
     * @param params
     * @param result
     *
     * @return bool
     */
    bool cmdViewStatus(const string& command, const string& params, string& result);

    /**
     * 查看链接状态
     * @param command
     * @param params
     * @param result
     *
     * @return bool
     */
    bool cmdConnections(const string& command, const string& params, string& result);

    /**
     * 查看编译的版本
     * @param command
     * @param params
     * @param result
     * 
     * @return bool
     */
    bool cmdViewVersion(const string& command, const string& params, string& result);

    /**
     * 使配置文件的property信息生效
     * @param command
     * @param params
     * @param result
     * 
     * @return bool
     */
    bool cmdLoadProperty(const string& command, const string& params, string& result);
    
    /**
     *查看服务支持的管理命令
     * @param params
     * @param result
     * 
     * @return bool
     */
    bool cmdViewAdminCommands(const string& command, const string& params, string& result);

    /**
     * 设置染色消息
     * @param command
     * @param params
     * @param result
     * 
     * @return bool
     */
    bool cmdSetDyeing(const string& command, const string& params, string& result);


      /**
     * 设置是否关闭stdcout/stderr/stdin 服务重启能才生效
     * @param command
     * @param params
     * @param result
     *
     * @return bool
     */
    bool cmdCloseCout(const string& command, const string& params, string& result);

protected:
    /**
     * 为Adapter绑定对应的handle类型
     * 缺省实现是ServantHandle类型
     * @param adapter 
     */
    virtual void setHandle(TC_EpollServer::BindAdapterPtr& adapter);

    /**
     * 添加Servant
     * @param T
     * @param id
     */
    template<typename T> void addServant(const string &id)
    {
        ServantHelperManager::getInstance()->addServant<T>(id);
    }

    /**
     * 非taf协议server，设置Servant的协议解析器 
     * @param protocol 
     * @param servant 
     */
    void addServantProtocol(const string& servant, const TC_EpollServer::protocol_functor& protocol);

protected:
    /**
     * 读取基本信息
     */
    void initializeClient();

    /**
     * 输出
     * @param os
     */
    void outClient(ostream &os);

    /**
     * 初始化servant
     */
    void initializeServer();

    /**
     * 输出
     * @param os
     */
    void outServer(ostream &os);

    /**
     * 输出所有的adapter
     * @param os
     */
    void outAllAdapter(ostream &os);

    /**
     * 输出
     * @param os
     */
    void outAdapter(ostream &os, const string &v, TC_EpollServer::BindAdapterPtr lsPtr);

    /**
     * 解析配置文件
     */
    void parseConfig(int argc, char *argv[]);
    
     /**
     * 解析ip权限allow deny 次序
     */
    TC_EpollServer::BindAdapter::EOrder parseOrder(const string &s);

    /**
     * 绑定server配置的Adapter和对象
     */
    void bindAdapter(vector<TC_EpollServer::BindAdapterPtr>& adapters);

    /**
     * @param servant 
     * @param sPrefix 
     */
    void checkServantNameValid(const string& servant, const string& sPrefix);

    /**
     * 换成缺省值
     * @param s
     * 
     * @return string
     */
    string toDefault(const string &s, const string &sDefault);

protected:
    /**
     * 配置文件
     */
    static TC_Config _conf;

    /**
     * 服务
     */
    static TC_EpollServerPtr _epollServer;

    /**
     * 通信器
     */
    static CommunicatorPtr _communicator;
};
////////////////////////////////////////////////////////////////////
}

#endif

