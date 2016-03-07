#include "HelloImp.h"
#include "HelloServer.h"
#include "CCProtocol.h"

using namespace taf;

HelloImp::HelloImp()
{ 
}

int HelloImp::doRequest(taf::JceCurrentPtr current, vector<char>& response)
{
    ServantPrx prx;

    Application::getCommunicator()->stringToProxy(HelloServer::CCSERVER_OBJ, prx);

    const vector<char>& request = current->getRequestBuffer();

    string buf((const char*)(&request[0]), request.size());

    ResponsePacket rsp;

    CCProtocol::packc pc;

    uint32_t len = buf.length();

    pc.Decode(buf.c_str(), len);

    LOG->debug() << "[Adapter:" << current->getBindAdapter()->getName() << "]->C2S Cmd:" << pc.Cmd << ",Seq:" <<  pc.SeqNo << ",Len:" << pc.Len << endl;

    /*sync-call
    prx->rpc_call("Transfer", buf.c_str(), buf.length(), rsp, pc.SeqNo);
    cout << "end remote call,ret len:" << rsp.sBuffer.size() << endl;
    response = rsp.sBuffer;
    */

    /*a-sync-call*/
    ServantCallbackPtr cb = new ServantCallback("ServantCallback",this, current);
    prx->rpc_call_async(pc.SeqNo, "AysncTransfer", buf.c_str(), buf.length(), cb);
    current->setResponse(false);
    /**/

    return 0;
}

int HelloImp::doResponse(ReqMessagePtr resp)
{
    ServantCallback* cb = dynamic_cast<ServantCallback*>(resp->callback.get());

    if (cb)
    {
        vector<char>& buff = resp->response.sBuffer;

        if (!buff.empty())
        {
            CCProtocol::packs ps;

            uint32_t len = buff.size();

            ps.Decode((const char*)(&buff[0]), len);

            LOG->debug() << "[CBType:" << cb->getType() << ",FromObject:" << resp->proxy->taf_name() << "]<-S2C Cmd:" << ps.Cmd << ",Seq:" <<  ps.SeqNo << ",Len:" << ps.Len << endl;
            
            cb->getCurrent()->sendResponse(0, buff);
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void HelloImp::initialize()
{
    TAF_ADD_ADMIN_CMD_NORMAL("SETHELLO", HelloImp::procHello);

    //_handle->addFilter(this);
}

void HelloImp::destroy()
{
}

int HelloImp::testHello(const string &s, string &r, JceCurrentPtr current)
{
    r = "test hello succ+" + s;
    return 0;
}

bool HelloImp::procHello(const string& command, const string& params, string& result)
{
    result = "succ!";

    return false;
}
///////////////////////////////////////////////////////////////////////////////
