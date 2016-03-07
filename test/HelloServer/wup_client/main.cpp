#include "servant/Communicator.h"
#include "util/tc_common.h"
#include "util/tc_monitor.h"
#include <sys/un.h>
#include <iostream>
#include "Hello.h"
#include "log/taf_logger.h"
#include "jce/wup.h"
#include "util/tc_clientsocket.h"

using namespace std;
using namespace taf;
using namespace Test;
using namespace wup;

int main(int argc, char *argv[])
{
    try
	{
        TafUniPacket<> req;

        req.setRequestId(0);
        req.setServantName("Test.HelloServer.HelloObj");
        req.setFuncName("testHello");
        req.setTafVersion(2);

		req.put<string>("s", "xiyangyang");

        string buff = "";
        req.encode(buff);

        TC_TCPClient client("172.25.38.67", 45460, 5000);

        char recvBuff[1024] = {0};
        size_t recvLen = sizeof(recvBuff);

		string sendBuff = "";

		int32_t iHeaderLen = htonl(sizeof(int32_t) + buff.length());

		sendBuff.append((const char*)&iHeaderLen, sizeof(int32_t));

		sendBuff.append(buff);

        int ret = client.sendRecv(sendBuff.c_str(), sendBuff.length(), recvBuff, recvLen);

        cout << ret << ":" << recvLen << endl;

		TafUniPacket<> rsp;

		rsp.decode(recvBuff, recvLen);

		string retStr = "";

		ret = rsp.get<int32_t>("");

		rsp.get("r", retStr);

		cout << "resp:" << ret << ":" << retStr << endl;
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}
	cout << "main return." << endl;

	return 0;
}
