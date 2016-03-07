#include "hessian/HessianProxy.h"
#include "hessian/HessianOutput.h"
#include "hessian/HessianInput.h"
#include "util/tc_http.h"
#include <memory>
#include <sstream>
#include <cassert>
#include <stdlib.h>

using namespace std;

namespace taf
{

#define HESSIAN_HTTP_CONTENT_TYPE "application/binary"
#define HESSIAN_HTTP_USER_AGENT "hessian cpp"


UrlManage::UrlManage() : _iDefaultSeconds(10), _iSeconds(_iDefaultSeconds)
{
    srand(time(NULL));
}

void UrlManage::registryUrl(const string &url)
{
    _vUrl.push_back(url);
}

bool UrlManage::getRandomUrl(pair<vector<string>::iterator,  string> &stUrl)
{
    //存在无效的URL, 且_iSeconds没有使用了, 则恢复
    if(_vInvalidUrl.size() > 0 && ((time(NULL) - _vInvalidUrl[0].first) > _iSeconds))
    {
        _vUrl.push_back(_vInvalidUrl[0].second);
        _vInvalidUrl.erase(_vInvalidUrl.begin());
    }

    //无效的URL
    if(_vUrl.size() == 0)
    {
        _iSeconds   = 0;  //调整恢复的时间
        return false;
    }

    _iSeconds       = _iDefaultSeconds;
    stUrl.first     = _vUrl.begin() + rand() % _vUrl.size();
    stUrl.second    = *stUrl.first;

    return true;
}

void UrlManage::setInvalid(vector<string>::iterator it)
{
    _vInvalidUrl.push_back(make_pair(time(NULL), *it));
    _vUrl.erase(it);
}

void UrlManage::setInvalidSeconds(time_t iSeconds)
{
    _iDefaultSeconds  = iSeconds;
    _iSeconds         = _iDefaultSeconds;
}

//////////////////////////////////////////////////////////////////////
// 
// 

string HessianProxy::CLIENT_CHARSET = "gbk";

void HessianProxy::registryUrl(const string &url)
{
    _urlManage.registryUrl(url);
}

void HessianProxy::setInvalidSeconds(time_t iSeconds)
{
    _urlManage.setInvalidSeconds(iSeconds);
}

void HessianProxy::setUtf8()
{
    CLIENT_CHARSET = "utf8";
}

void HessianProxy::setGbk()
{
    CLIENT_CHARSET = "gbk";
}

bool HessianProxy::isUtf8()
{
    return CLIENT_CHARSET == "utf8";
}

bool HessianProxy::isGbk()
{
    return CLIENT_CHARSET == "gbk";
}

HObjectPtr HessianProxy::call(const string& method, vector<HObjectPtr> &vParam)
{
    _buffer = "";

    HessianOutput hOut;

    _buffer = hOut.start(method);
    for(size_t i = 0; i < vParam.size(); i++)
    {
        hOut.setParameter(_buffer, vParam[i]);
    }
    hOut.complete(_buffer);

    taf::TC_HttpRequest  httpRequest;
    taf::TC_HttpResponse httpResponse;
    httpRequest.setUserAgent(HESSIAN_HTTP_USER_AGENT);
    httpRequest.setContentType(HESSIAN_HTTP_CONTENT_TYPE);

begin:
    pair<vector<string>::iterator,  string> url;
    bool b = _urlManage.getRandomUrl(url);
    if(!b)
    {
        throw HessianNoActiveServiceException("HessianNoActiveServiceException::no active url, please wait for several seconds.");
    }

    try
    {
        httpRequest.setPostRequest(url.second, _buffer);
        int ret = httpRequest.doRequest(httpResponse);
        throwHessianException(ret);
    }
    catch(HessianConnectException &ex)
    {
        //连接不上服务器, 切换到其他URL
        _urlManage.setInvalid(url.first);
        goto begin;
    }

    HessianInput hIn(httpResponse.getContent());
    HObjectPtr n = hIn.result();
    HFaultPtr f = HFaultPtr::dynamicCast(n);
    if(f)
    {
        f->throwException();
    }
    return n;
}

}


