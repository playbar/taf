#ifndef __TAF_ADMIN_SERVANT_H
#define __TAF_ADMIN_SERVANT_H

#include "servant/AdminF.h"

namespace taf
{

class JceCurrent;
class Application;

/**
 * 管理Servant
 */
class AdminServant : public AdminF
{
public:
    /**
     * 构造函数
     * @param pEpollServer
     */
    AdminServant();

    /**
     * 析构函数
     */
    virtual ~AdminServant();

    /**
     * 初始化
     * @return int
     */
    virtual void initialize();

    /**
     * 退出
     */
    virtual void destroy();

    /**
     * 关闭服务
     * @param current
     */
    void shutdown(JceCurrentPtr current);

    /**
     * 通知
     * @param command
     */
    string notify(const string &command, JceCurrentPtr current);

};
////////////////////////////////////////////////////////////////
}

#endif


