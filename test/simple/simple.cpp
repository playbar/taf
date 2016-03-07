#include "echo.h"
#include "log/taf_logger.h"

using namespace std;
using namespace taf;


class TCServer : public TC_EpollServer
{
public:
    template<typename T>
    void bind(const string &str)
    {
//        T *pHandle = new T;

        TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(this);

        //设置adapter名称, 唯一
        lsPtr->setName(str);
        //设置绑定端口
        lsPtr->setEndpoint(str);
        //设置最大连接数
        lsPtr->setMaxConns(20000);
        //设置启动线程数
        lsPtr->setHandleNum(10);
        //设置协议解析器
        lsPtr->setProtocol(&T::parse);

        //绑定对象
        TC_EpollServer::bind(lsPtr);

        //设置逻辑处理器
        lsPtr->setHandle<T>();
    }
};

TC_RollLogger g_logger;
TCServer       g_app;

int main(int argc, char *argv[])
{
    try
	{
        g_app.setLocalLogger(&g_logger);

        g_app.bind<EchoHandle>("tcp -h 127.0.0.1 -p 8082 -t 5000");

        g_app.waitForShutdown();
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}

