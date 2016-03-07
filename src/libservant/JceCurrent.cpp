#include "servant/JceCurrent.h"
#include "servant/ServantHandle.h"
#include "servant/BaseF.h"
#include "servant/Application.h"
#include "jce/wup.h"
#include <cerrno>

namespace taf
{
//////////////////////////////////////////////////////////////////
JceCurrent::JceCurrent(ServantHandle *pServantHandle)
: _pServantHandle(pServantHandle)
, _ip("NULL")
, _port(0)
, _bResponse(true)
, _iRet(0)
{
}

JceCurrent::~JceCurrent()
{
    //WUP调用或单向调用，从服务端上报调用信息
    if(_request.iVersion == WUPVERSION || _request.iVersion == WUPVERSION2)
    {
        reportToStat("wup_client");
    }
    else if(_request.cPacketType == JCEONEWAY)
    {
        reportToStat("one_way_client");
    }
    else if(!_pBindAdapter->isTafProtocol() && ServerConfig::ReportFlow)
    {
        //非taf客户端 从服务端上报调用信息
        reportToStat("not_taf_client");
    }
}

string JceCurrent::getIp() const
{ 
    return _ip; 
}

int JceCurrent::getPort() const
{ 
    return _port;
}

string JceCurrent::getServantName() const
{ 
    return _request.sServantName; 
}

short JceCurrent::getRequestVersion() const
{ 
    return _request.iVersion; 
}

map<string, string>& JceCurrent::getContext()
{ 
    return _request.context;
}

const map<string, string>& JceCurrent::getRequestStatus() const 
{ 
    return _request.status; 
}

string JceCurrent::getFuncName() const
{ 
    return _request.sFuncName; 
}

uint32_t JceCurrent::getRequestId() const
{ 
    return _request.iRequestId; 
}

char JceCurrent::getPacketType() const
{ 
    return _request.cPacketType; 
}

taf::Int32 JceCurrent::getMessageType() const
{ 
    return _request.iMessageType; 
}

struct timeval JceCurrent::getRecvTime() const
{
    return _begintime;
}

const vector<char>& JceCurrent::getRequestBuffer() const   
{ 
    return _request.sBuffer; 
}

bool JceCurrent::isResponse() const
{ 
    return _bResponse; 
}

void JceCurrent::initialize(const TC_EpollServer::tagRecvData &stRecvData)
{
    _ip = stRecvData.ip;

    _port = stRecvData.port;

    _uid = stRecvData.uid;

    _pBindAdapter = stRecvData.adapter.get();

    _request.sServantName = ServantHelperManager::getInstance()->getAdapterServant(stRecvData.adapter->getName());

    if (_pBindAdapter->isTafProtocol())
    {
        initialize(stRecvData.buffer);
    }
    else
    {
        //if (ServerConfig::ReportFlow)
        //{
        //    TC_TimeProvider::getInstance()->getNow(&_begintime);
        //}
        _request.sBuffer.resize(stRecvData.buffer.length());

        ::memcpy(&_request.sBuffer[0], stRecvData.buffer.c_str(), stRecvData.buffer.length());
    }

    TC_TimeProvider::getInstance()->getNow(&_begintime);
}

void JceCurrent::initialize(const string &sRecvBuffer)
{
    JceInputStream<BufferReader> is;

    is.setBuffer(sRecvBuffer.c_str(), sRecvBuffer.length());

    _request.readFrom(is);

    //wup调用及单向调用由服务端上报计算调用信息
    //非taf协议服务端上报
    //if (_request.iVersion == WUPVERSION || _request.iVersion == WUPVERSION2 || _request.cPacketType == JCEONEWAY )
    //{
    //   TC_TimeProvider::getInstance()->getNow(&_begintime);
    //}
}
void JceCurrent::sendResponse(const char* buff, uint32_t len)
{
    _pServantHandle->sendResponse(_uid, string(buff, len), _ip, _port);
}

void JceCurrent::sendResponse(int iRet, const vector<char>& buffer, const map<string, string>& status, const string & sResultDesc)
{
    _iRet = iRet;

    //单向调用不需要返回
    if (_request.cPacketType == JCEONEWAY)
    {
        return;
    }
    
    JceOutputStream<BufferWriter> os;

    if (_request.iVersion != WUPVERSION && _request.iVersion != WUPVERSION2)
    {
        ResponsePacket response;

        response.iRequestId     = _request.iRequestId;
        response.iMessageType   = _request.iMessageType;
        response.cPacketType    = JCENORMAL;
        response.iVersion       = JCEVERSION;
        response.status         = status;
        response.sBuffer        = buffer;
        response.sResultDesc    = sResultDesc;
        response.iRet           = iRet;

        response.writeTo(os);
    }
    else
    {
        //wup回应包用请求包的结构
        RequestPacket response;

        response.iRequestId     = _request.iRequestId;
        response.iMessageType   = _request.iMessageType;
        response.cPacketType    = JCENORMAL;
        response.iVersion       = _request.iVersion;
        response.status         = status;
        response.sBuffer        = buffer;
        response.sServantName   = _request.sServantName;
        response.sFuncName      = _request.sFuncName;

        //异常的情况下buffer可能为空，要保证有一个空UniAttribute的编码内容
        if(response.sBuffer.size() == 0)
        {
            wup::UniAttribute<> tafAttr;
			tafAttr.setVersion(_request.iVersion);
            tafAttr.encode(response.sBuffer);
        }
        //iRet为0时,不记录在status里面,和wup客户端协议一致,节省空间
        if(iRet != 0)
        {
            response.status[ServantProxy::STATUS_RESULT_CODE] = TC_Common::tostr(iRet);
        }
        //sResultDesc为空时,不记录在status里面,和wup客户端协议一致,节省空间
        if(!sResultDesc.empty())
        {
            response.status[ServantProxy::STATUS_RESULT_DESC] = sResultDesc;
        }
        response.writeTo(os);
    }

    taf::Int32 iHeaderLen = htonl(sizeof(taf::Int32) + os.getLength());

    string s = "";

    s.append((const char*)&iHeaderLen, sizeof(taf::Int32));

    s.append(os.getBuffer(), os.getLength());

    _pServantHandle->sendResponse(_uid, s, _ip, _port);
}

void JceCurrent::close()
{
    if (_pServantHandle)
    {
        _pServantHandle->close(_uid);
    }
}

ServantHandle* JceCurrent::getServantHandle()
{
    return _pServantHandle;
}

TC_EpollServer::BindAdapter* JceCurrent::getBindAdapter()
{
    return _pBindAdapter;
}

void JceCurrent::reportToStat(const string& sObj)
{
    StatReport* stat = Application::getCommunicator()->getStatReport();

    if(stat && stat->getStatPrx())
    {
        timeval endtime;

        int sptime = 10*1000;

        //gettimeofday(&endtime, NULL);
        TC_TimeProvider::getInstance()->getNow(&endtime);

        if (_begintime.tv_sec != 0
            && endtime.tv_sec != 0
            && endtime.tv_sec >= _begintime.tv_sec)
        {
            sptime = (endtime.tv_sec - _begintime.tv_sec)*1000;
            sptime += (endtime.tv_usec - _begintime.tv_usec)/1000;
        }
        stat->report(sObj, _request.sFuncName, _ip, 0, (StatReport::StatResult)_iRet, sptime, 0, false);
    }
}

void JceCurrent::saveSampleKey()
{
    ServantProxyThreadData* sptd = ServantProxyThreadData::getData();  

    if (!sptd)
    {
        return;
    }
    sptd->data()->_sampleKey._root        = true;  //默认为根节点
    sptd->data()->_sampleKey._width       = 0;
    sptd->data()->_sampleKey._depth       = 0;
    sptd->data()->_sampleKey._unid        = "";
    sptd->data()->_sampleKey._parentWidth = -1; //自服务框架JceCurrent初始化 父节点广度设置为-1
    
    bool isSampleMessage  = IS_MSG_TYPE(_request.iMessageType, taf::JCEMESSAGETYPESAMPLE);           

    if (isSampleMessage)
    {
        sptd->data()->_sampleKey._root = false;   //已经是采样消息了, 则非跟节点

        map<string, string>::const_iterator keyIt = _request.status.find(ServantProxy::STATUS_SAMPLE_KEY);

        if(keyIt != _request.status.end())
        {
            //status[ServantProxy::STATUS_SAMPLE_KEY]="id|depth|width"形式.合在一起以便节省编解码
            vector<string> v = TC_Common::sepstr<string>(keyIt->second,"|"); 

            if(v.size() > 2)
            {
                sptd->data()->_sampleKey._unid        = v[0];
                sptd->data()->_sampleKey._depth       = TC_Common::strto<int>(v[1])+1;   //深度+1  
                sptd->data()->_sampleKey._parentWidth = TC_Common::strto<int>(v[2]);            
            }
        }
    } 
}
////////////////////////////////////////////////////////////////////////////
}
