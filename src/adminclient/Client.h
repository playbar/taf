#ifndef __ONE_ADMIN_CLIENT_H__
#define __ONE_ADMIN_CLIENT_H__

#include "util/tc_common.h"
#include "util/tc_option.h"
#include "util/tc_config.h"

#include "servant/Communicator.h"
#include "AdminReg.h"
#include "servant/QueryF.h"
#include "Node.h"
#include "Parser.h"

#include <iostream>

using namespace taf;

class Client
{
    public:
        /**
         * 初始化管理端口
         * @param conf 配置文件
         * @return
         */
        int init(TC_Config & conf);

        /**
         * notify本地admin对象
         * @param ipport 
         * @param command 命令
         * @param result 结果
         * @return
         */
        int doNotify(const string & ipport, const string & command, string & result);

        /**
         * 使用方法
         * @param 命令行启动参数
         */
        void usage(int argc, char *argv[]);

        /**
         * 处理过程
         */
        int run();

    protected:
        taf::CommunicatorPtr _comm;

        AdminRegPrx _adminPtr;
        NodePrx _nodePtr;

};



#endif
